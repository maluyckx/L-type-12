#include "GameServer.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <filesystem>
#include "../../Utils/DebugLogger.hpp"
#include <csignal>
#include "../Client/Window.hpp"

void signal_handler(int) {
    Game::Server::GameServer::destroyLockFile();
    exit(EXIT_FAILURE);
}

Game::Server::GameServer::GameServer() : _database(new Database::Database()) {
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGHUP, signal_handler);
    if (!createLockFile()) {
        std::cerr << "Another instance of " << GameServer::process_name
                  << " is running!" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (unsigned i = 0; i < MAX_SESSIONS; ++i) {
        _id_logged_in[i][USER_1] = NO_USER_ID;
        _id_logged_in[i][USER_2] = NO_USER_ID;
    }
}

[[maybe_unused]]
Game::Server::GameServer::GameServer(Game::Server::GameServer&& other) noexcept:
    AppInstance(other),
    _client_to_matchmaker(other._client_to_matchmaker),
    _matchmaker_to_client(other._matchmaker_to_client),
    _database(other._database) {
    for (unsigned i = 0; i < MAX_SESSIONS; ++i) {
        _current_sessions[i]       = other._current_sessions[i];
        other._current_sessions[i] = nullptr;
        _id_logged_in[i][USER_1]   = other._id_logged_in[i][USER_1];
        _id_logged_in[i][USER_2]   = other._id_logged_in[i][USER_2];
    }
}

Game::Server::GameServer&
Game::Server::GameServer::operator=(Game::Server::GameServer&& rhs) noexcept {
    _client_to_matchmaker = rhs._client_to_matchmaker;
    _matchmaker_to_client = rhs._matchmaker_to_client;
    _database             = rhs._database;
    rhs._database = nullptr;
    for (unsigned i = 0; i < MAX_SESSIONS; ++i) {
        _current_sessions[i]     = rhs._current_sessions[i];
        rhs._current_sessions[i] = nullptr;
        _id_logged_in[i][USER_1] = rhs._id_logged_in[i][USER_1];
        _id_logged_in[i][USER_2] = rhs._id_logged_in[i][USER_2];
    }
    return *this;
}

Game::Server::GameServer::~GameServer() noexcept {
    destroyLockFile();
    for (auto& _current_session : _current_sessions) {
        delete _current_session;
    }
    delete _database;
}

void Game::Server::GameServer::run() {
    //create the pipes
    if (!std::filesystem::exists(_client2server_matchmaker_pipe_path)) {
        DEBUGMSG("Creating pipe: " << _client2server_matchmaker_pipe_path);
        if (mkfifo(_client2server_matchmaker_pipe_path, 0666)) {
            std::cerr << "Couldn't create pipe "
                      << _client2server_matchmaker_pipe_path << std::endl;
            delete this;
            exit(EXIT_FAILURE);
        }
    }
    if (!std::filesystem::exists(_server2client_matchmaker_pipe_path)) {
        DEBUGMSG("Creating pipe: " << _server2client_matchmaker_pipe_path);
        if (mkfifo(_server2client_matchmaker_pipe_path, 0666)) {
            std::cerr << "Couldn't create pipe "
                      << _server2client_matchmaker_pipe_path << std::endl;
            delete this;
            exit(EXIT_FAILURE);
        }
    }
    listen();
}

void Game::Server::GameServer::listen() {
    DEBUGMSG("Listening...");
    pid_t               client_pid = 0;
    Client::DisplayMode display_mode{};
    for (;;) {
        DEBUGMSG("Opening pipe " << _client2server_matchmaker_pipe_path);
        _client_to_matchmaker = open(_client2server_matchmaker_pipe_path,
                                     O_RDONLY);
        long bytes_read       = read(_client_to_matchmaker, &client_pid,
                                     sizeof(pid_t));
        bool read_failed_pid  = bytes_read < 0;
        bytes_read = read(_client_to_matchmaker, &display_mode,
                          sizeof(Client::DisplayMode));
        bool read_failed_gfx = bytes_read < 0;
        if (read_failed_pid || read_failed_gfx) {
            std::cerr << "An error occured when reading from "
                         "" << _client_to_matchmaker << std::endl;
            close(_client_to_matchmaker);
        } else {
            DEBUGMSG("Client connected to "
                         << _client2server_matchmaker_pipe_path);
            cleanTerminatedSessions();
            unsigned session_id = getFirstFreeSessionId();
            DEBUGMSG("Opening pipe " << _server2client_matchmaker_pipe_path);
            _matchmaker_to_client = open(_server2client_matchmaker_pipe_path,
                                         O_WRONLY);
            DEBUGMSG("Client connected to "
                         << _server2client_matchmaker_pipe_path);
            if (session_id == MAX_SESSIONS) {
                // we can't spawn more game sessions
                session_id = BAD_SESSION;
                long bytes_written = write(_matchmaker_to_client, &session_id,
                                           sizeof(unsigned));
                if (bytes_written < 0) {
                    // if we can't write to the pip, shits hit the fan,
                    // kill the client, I mean notify the client to commit
                    // seppuku 
                    kill(client_pid, SIGPIPE);
                }
            } else {
                auto new_session   = new GameSession(this, session_id,
                                                     client_pid, display_mode);
                long bytes_written = write(_matchmaker_to_client, &session_id,
                                           sizeof(unsigned));
                if (bytes_written < 0) {
                    kill(client_pid, SIGPIPE);
                    delete new_session;
                } else {
                    _current_sessions[session_id] = new_session;
                    DEBUGMSG("Starting new session ..." << session_id);
                    if (!new_session->start()) {
                        DEBUGERR("Starting new session "
                                     << session_id << " failed!");
                        endSession(session_id);
                    }
                }
            }
            close(_matchmaker_to_client);
            close(_client_to_matchmaker);
            sleep(1);
        }
    }
}

inline unsigned Game::Server::GameServer::getFirstFreeSessionId() {
    unsigned i = 0;
    while (i < MAX_SESSIONS && _current_sessions[i]) { ++i; }
    return i;
}

void Game::Server::GameServer::cleanTerminatedSessions() {
    for (unsigned i = 0; i < MAX_SESSIONS; ++i) {
        if (_current_sessions[i] &&
            _current_sessions[i]->getState() == GameSessionState::Terminated) {
            endSession(i);
        }
    }
}

void Game::Server::GameServer::endSession(unsigned int id) {
    if (_current_sessions[id]) {
        DEBUGMSG("Ending Session (" << id << ")");
        kill(_current_sessions[id]->getConnection()->getClientPID(), SIGPIPE);
        auto temp = _current_sessions[id];
        _current_sessions[id] = nullptr;
        delete temp;
        removeAllUserIDLoggedInSession(id);
    }
}

bool Game::Server::GameServer::createLockFile() {
#if DEBUG
    // when in debug, we can't catch signals
    destroyLockFile();
#endif
    // wx forces to create the file, if it already exists, fopen returns 0
    // use remove when the server shutdown
    DEBUGMSG("Creating lock " << lock_file_path);
    return fopen(lock_file_path, "wx");
}

void Game::Server::GameServer::destroyLockFile() {
    DEBUGMSG("Destroying lock " << lock_file_path);
    if (std::filesystem::exists(lock_file_path)) { remove(lock_file_path); }
}

bool Game::Server::GameServer::isUserIDLoggedIn(int user_id) {
    bool          isLoggedIn = false;
    for (unsigned i          = 0; !isLoggedIn && i < MAX_SESSIONS; ++i) {
        if (_id_logged_in[i][USER_1] == user_id ||
            _id_logged_in[i][USER_2] == user_id) {
            isLoggedIn = true;
        }
    }
    return isLoggedIn;
}

void Game::Server::GameServer::addUserIDLoggedIn(unsigned int session_id,
                                                 int user_id) {
    DEBUGMSG("Session(" << session_id << "): Logging in user = " << user_id);
    if (_id_logged_in[session_id][USER_1] == NO_USER_ID) {
        _id_logged_in[session_id][USER_1] = user_id;
    } else if (_id_logged_in[session_id][USER_2] == NO_USER_ID) {
        _id_logged_in[session_id][USER_2] = user_id;
    }
}

void Game::Server::GameServer::removeUserIDLoggedIn(unsigned int session_id,
                                                    int user_id) {
    DEBUGMSG("Session(" << session_id << "): Logging out user = " << user_id);
    if (_id_logged_in[session_id][USER_1] == user_id) {
        _id_logged_in[session_id][USER_1] = NO_USER_ID;
    } else if (_id_logged_in[session_id][USER_2] == user_id) {
        _id_logged_in[session_id][USER_2] = NO_USER_ID;
    }
}

void
Game::Server::GameServer::removeAllUserIDLoggedInSession(
    unsigned int session_id) {
    DEBUGMSG("Session(" << session_id << "): Logging out all users from "
                                         "session");
    _id_logged_in[session_id][USER_1] = NO_USER_ID;
    _id_logged_in[session_id][USER_2] = NO_USER_ID;
}
