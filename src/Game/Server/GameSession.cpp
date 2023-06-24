#include "GameSession.hpp"
#include "../../Utils/DebugLogger.hpp"
#include "../Command.hpp"
#include <csignal>
#include "GameServer.hpp"
#include "../Client/Window.hpp"


Game::Server::GameSession::GameSession(GameServer* server, unsigned session_id,
                                       pid_t client_pid,
                                       Client::DisplayMode mode) :
    _id(session_id), _state(GameSessionState::NotStarted),
    _state_mutex(new pthread_mutex_t()),
    _server(server), _thread(new pthread_t()),
    _connection(new Connection(this, _id, client_pid)),
    _current_game(nullptr),
    _display_mode(mode) {
    pthread_mutex_init(_state_mutex, nullptr);
}

Game::Server::GameSession::GameSession(
    Game::Server::GameSession&& other) noexcept:
    _id(other._id), _state(other._state),
    _state_mutex(other._state_mutex),
    _server(other._server),
    _thread(other._thread),
    _connection(other._connection),
    _current_game(other._current_game),
    _display_mode(other._display_mode) {
    other._state_mutex  = nullptr;
    other._server       = nullptr;
    other._thread       = nullptr;
    other._connection   = nullptr;
    other._current_game = nullptr;
}

Game::Server::GameSession&
Game::Server::GameSession::operator=(Game::Server::GameSession&& rhs) noexcept {
    _id           = rhs._id;
    _state        = rhs._state;
    _state_mutex  = rhs._state_mutex;
    _server       = rhs._server;
    _thread       = rhs._thread;
    _connection   = rhs._connection;
    _current_game = rhs._current_game;
    _display_mode = rhs._display_mode;
    rhs._state_mutex  = nullptr;
    rhs._server       = nullptr;
    rhs._thread       = nullptr;
    rhs._connection   = nullptr;
    rhs._current_game = nullptr;
    return *this;
}

Game::Server::GameSession::~GameSession() noexcept {
    pthread_mutex_destroy(_state_mutex);
    delete _state_mutex;
    delete _thread;
    delete _connection;
}

[[maybe_unused]] bool Game::Server::GameSession::isDisplayTerminal() const {
    return _display_mode == Client::DisplayMode::Terminal;
}

bool Game::Server::GameSession::isDisplayGraphical() const {
    return _display_mode == Client::DisplayMode::Graphical;
}

Database::Database*
Game::Server::GameSession::getDatabase() const noexcept {
    return _server->getDatabase();
}

bool Game::Server::GameSession::start() {
    bool started = false;
    if (_connection->open()) {
        if (pthread_create(_thread, nullptr, run, this)) {
            std::cerr << "Failed to start session(" << _id << ") thread!"
                      << std::endl;
            kill(_connection->getClientPID(), SIGPIPE);
        } else {
            started = true;
            pthread_detach(*_thread);
            setState(GameSessionState::Running);
        }
    }
    return started;
}

void* Game::Server::GameSession::run(void* args) {
    auto             session                 = static_cast<GameSession*>(args);
    auto const       receiving_command_queue =
                         session->getConnection()->getReceivingCommandQueue();
    Utils::StopWatch chrono{};
    // main loop of the session
    for (;;) {
        chrono.start();
        if (session->_connection->getState() == ConnectionState::Closed) {
            session->setState(GameSessionState::Terminated);
        }
        if (session->getState() == GameSessionState::Terminated) {
            session->logOutAllUsers();
            pthread_exit(nullptr);
        }
        // process commands from client
        while (!receiving_command_queue->empty()) {
            CommandExecutor::execute(session->_connection,
                                     receiving_command_queue->pop());
        }
        // run the update routine of the game
        session->updateGameInstance();
        chrono.stop();
        unsigned delta_time_ms = static_cast<unsigned>(
            chrono.getElapsedSecs() * 1000
        );
        // if we work too fast the pipes aren't keeping up, also to normilise
        // the thick speed of the server provide a more consistent gameplay
        // logic
        unsigned sleep_time_ms = SERVER_TICK_TIME_MS > delta_time_ms ?
                                 SERVER_TICK_TIME_MS - delta_time_ms : 0u;
        usleep(sleep_time_ms * 1000);
        DEBUGMSG("Time (SESSION=" << session->_id << "): " <<
                                  chrono.getElapsedSecs() * 1000 + sleep_time_ms
                                  << " ms");
    }
}

void Game::Server::GameSession::startGame(char* username_player1,
                                          char* username_player2,
                                          Game::GameSettings settings) {
    setGameInstance(
        new Game::GameInstance(
            username_player1,
            username_player2,
            settings,
            _connection
        )
    );
}

void Game::Server::GameSession::endGame() {
    delete getGameInstance();
    setGameInstance(nullptr);
}

bool Game::Server::GameSession::isUserLoggedIn(int user_id) {
    return _server->isUserIDLoggedIn(user_id);
}

void Game::Server::GameSession::logInUser(int user_id) {
    _server->addUserIDLoggedIn(_id, user_id);
}

void Game::Server::GameSession::logOutUser(int user_id) {
    _server->removeUserIDLoggedIn(_id, user_id);
}

void Game::Server::GameSession::logOutAllUsers() {
    _server->removeAllUserIDLoggedInSession(_id);
}
