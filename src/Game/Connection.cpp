#include "Connection.hpp"
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <csignal>
#include <memory.h>
#include "Command.hpp" // this include needs to be after Connection to avoid circular imports
#include <filesystem>
#include "../Utils/DebugLogger.hpp"
#include <cerrno>
#include <iostream>

Game::Connection::Connection(void* owner, unsigned int session_id,
                             pid_t client_pid) :
        _owner(owner),
        _state(ConnectionState::Closed),
        _state_mutex(new pthread_mutex_t()),
        _session_id(session_id),
        _client_pid(client_pid) {
    pthread_mutex_init(_state_mutex, nullptr);
    sprintf(
            const_cast<char*>(_client_to_session_pipe_path),
            "/tmp/ltype_gr12_c(%u)2s(%u)_pipe",
            client_pid,
            session_id
    );
    sprintf(
            const_cast<char*>(_session_to_client_pipe_path),
            "/tmp/ltype_gr12_s(%u)2c(%u)_pipe",
            session_id,
            client_pid
    );
#ifdef SERVER
    // create the pipes
    if (!std::filesystem::exists(_client_to_session_pipe_path)) {
        mkfifo(_client_to_session_pipe_path, 0666);
    }
    if (!std::filesystem::exists(_session_to_client_pipe_path)) {
        mkfifo(_session_to_client_pipe_path, 0666);
    }
#else

#endif
}

Game::Connection::Connection(Game::Connection&& other) noexcept:
        _owner(other._owner), _state(other._state),
        _state_mutex(other._state_mutex),
        _session_id(other._session_id),
        _client_pid(other._client_pid),
        _client_to_session(other._client_to_session),
        _reading_thread(other._reading_thread),
        _writing_thread(other._writing_thread),
        _client_to_session_pipe_path(other._client_to_session_pipe_path),
        _session_to_client_pipe_path(other._session_to_client_pipe_path),
        _sending_command_queue(other._sending_command_queue),
        _receive_command_queue(other._receive_command_queue) {
    other._state_mutex                 = nullptr;
    other._reading_thread              = nullptr;
    other._writing_thread              = nullptr;
    other._client_to_session_pipe_path = nullptr;
    other._session_to_client_pipe_path = nullptr;
    other._sending_command_queue       = nullptr;
    other._receive_command_queue       = nullptr;
}

Game::Connection& Game::Connection::operator=(Game::Connection&& rhs) noexcept {
    _owner                       = rhs._owner;
    _state                       = rhs._state;
    _state_mutex                 = rhs._state_mutex;
    _session_id                  = rhs._session_id;
    _client_pid                  = rhs._client_pid;
    _client_to_session           = rhs._client_to_session;
    _session_to_client           = rhs._session_to_client;
    _reading_thread              = rhs._reading_thread;
    _writing_thread              = rhs._writing_thread;
    _client_to_session_pipe_path = rhs._client_to_session_pipe_path;
    _session_to_client_pipe_path = rhs._session_to_client_pipe_path;
    _sending_command_queue       = rhs._sending_command_queue;
    _receive_command_queue       = rhs._receive_command_queue;
    rhs._owner                       = nullptr;
    rhs._state_mutex                 = nullptr;
    rhs._reading_thread              = nullptr;
    rhs._writing_thread              = nullptr;
    rhs._client_to_session_pipe_path = nullptr;
    rhs._session_to_client_pipe_path = nullptr;
    rhs._sending_command_queue       = nullptr;
    rhs._receive_command_queue       = nullptr;
    return *this;
}

Game::Connection::~Connection() noexcept {
    close();
//    sleep(1); // give enough time for connection threads to end
    pthread_mutex_destroy(_state_mutex);
    delete _state_mutex;
    delete _reading_thread;
    delete _writing_thread;
    delete[] _client_to_session_pipe_path;
    delete[] _session_to_client_pipe_path;
    delete _sending_command_queue;
    delete _receive_command_queue;
}

bool Game::Connection::open() {
    DEBUGMSG("Opening Connection...");
    bool opened = false;
    if (pthread_create(_reading_thread, nullptr, receive, this) ||
        pthread_create(_writing_thread, nullptr, send, this)) {
        std::cerr << "Failed to open connection!" << std::endl;
        kill(getClientPID(), SIGPIPE);
    } else {
        opened = true;
        setState(ConnectionState::Open);
        pthread_detach(*_reading_thread);
        pthread_detach(*_writing_thread);
    }
    return opened;
}

bool Game::Connection::close() {
    setState(ConnectionState::Closed);
    DEBUGMSG("Closing Connection...");
    _sending_command_queue->clear();
    _receive_command_queue->clear();
    bool success = true;
    if (::close(_client_to_session)) { success = false; }
    if (::close(_session_to_client)) { success = false; }
//    if (pthread_cancel(*_writing_thread)) { success = false; }
//    if (pthread_cancel(*_reading_thread)) { success = false; }
    return success;
}

void* Game::Connection::receive(void* args) {
    auto connection              = static_cast<Connection*>(args);
    auto receiving_command_queue = connection->getReceivingCommandQueue();
    char buffer[sizeof(Command)];
    // TODO: when done with the function, reduce the ifdef to the minimum difference
#ifdef SERVER
    connection->_client_to_session = ::open(
            connection->_client_to_session_pipe_path, O_RDONLY);
    if (connection->_client_to_session < 0) {
        DEBUGERR("Opening pipe to client for reading "
                         << connection->_client_to_session_pipe_path
                         << " failed. "
                         << strerror(errno));
    }
    DEBUGMSG("Opening pipe to client for reading "
                     << connection->_client_to_session_pipe_path);
    for (;;) {
        if (connection->getState() == ConnectionState::Open) {
            memset(buffer, 0, sizeof(Command));
            long bytes_read = read(connection->_client_to_session, buffer,
                                   sizeof(Command));
            if (bytes_read < 0) {
                std::cerr << "Failed to read command from pipe. Command was "
                             "dropped!" << std::endl;
                connection->close();
            } else if (bytes_read == 0) {
                // the client was terminated, close the connection
                connection->close();
            } else {
                receiving_command_queue->push(
                        CommandFactory::deserialize(buffer));
                DEBUGMSG("Session(" << connection->_session_id
                                    << "): Command was pushed to recieving queue.");
            }
        } else {
            connection->getOwner<Server::GameSession>()->setState
                    (Server::GameSessionState::Terminated);
            pthread_exit(nullptr);
        }
    }
#else
    // TODO: should we loop until we are sure the pipes have been created by the session?
    sleep(1);
    connection->_session_to_client = ::open(
            connection->_session_to_client_pipe_path, O_RDONLY);
    if (connection->_client_to_session < 0) {
        DEBUGERR("Opening pipe to server for reading "
                         << connection->_session_to_client_pipe_path
                         << " failed! " << strerror(errno));
        connection->close();
    } else {
        DEBUGMSG("Opening pipe to server for reading "
                         << connection->_session_to_client_pipe_path);
    }
    for (;;) {
        if (connection->getState() == ConnectionState::Open) {
            memset(buffer, 0, sizeof(Command));
            long bytes_read = read(connection->_session_to_client, buffer,
                                   sizeof(Command));
            if (bytes_read < 0) {
                std::cerr << "Failed to read command from pipe. Command was "
                             "dropped! " << strerror(errno) << std::endl;
                connection->close();
            } else if (bytes_read == 0) {
                connection->close();
            } else {
                receiving_command_queue->push(
                        CommandFactory::deserialize(buffer));
            }
        } else {
            pthread_exit(nullptr);
        }
    }
#endif
}

void* Game::Connection::send(void* args) {
    auto connection                = static_cast<Connection*>(args);
    auto sending_command_queue     = connection->getSendingCommandQueue();
    char buffer[sizeof(Command)];
    // TODO: ifdef to minimum diff when done with function
#ifdef SERVER
    connection->_session_to_client = ::open(
            connection->_session_to_client_pipe_path, O_WRONLY);
    if (connection->_session_to_client < 0) {
        DEBUGERR("Opening pipe to client for writing "
                         << connection->_session_to_client_pipe_path
                         << " failed. "
                         << strerror(errno));
    }
    DEBUGMSG("Opening pipe to client for writing "
                     << connection->_session_to_client_pipe_path);
    for (;;) {
        if (connection->getState() == ConnectionState::Open) {
            if (!sending_command_queue->empty()) {
                memset(buffer, 0, sizeof(Command));
                CommandFactory::serialize(sending_command_queue->pop(), buffer);
                long bytes_written = write(connection->_session_to_client,
                                           buffer,
                                           sizeof(Command));
                if (bytes_written < 0) {
                    std::cerr << "Failed to write command to pipe. Command was "
                                 "dropped!" << std::endl;
                    connection->close();
                } else if (bytes_written == 0) {
                    connection->close();
                } else {
//                    DEBUGMSG("Session(" << connection->_session_id
//                                        << "): Command was pushed to sending queue.");
                }
            }
        } else {
            connection->getOwner<Server::GameSession>()->setState
                    (Server::GameSessionState::Terminated);
            pthread_exit(nullptr);
        }
    }
#else
    sleep(1);
    connection->_client_to_session = ::open(
            connection->_client_to_session_pipe_path, O_WRONLY);
    if (connection->_client_to_session < 0) {
        DEBUGERR("Opening pipe to server for writing "
                         << connection->_client_to_session_pipe_path
                         << " failed! " << strerror(errno));
        connection->close();
    } else {
        DEBUGMSG("Opening pipe to server for writing "
                         << connection->_client_to_session_pipe_path);
    }
    for (;;) {
        if (connection->getState() == ConnectionState::Open) {
            if (!sending_command_queue->empty()) {
                memset(buffer, 0, sizeof(Command));
                CommandFactory::serialize(sending_command_queue->pop(), buffer);
                long bytes_written = write(connection->_client_to_session,
                                           buffer,
                                           sizeof(Command));
                if (bytes_written < 0) {
                    std::cerr << "Failed to write command to pipe. Command was "
                                 "dropped! " << strerror(errno) << std::endl;
                } else if (bytes_written == 0) {
                    connection->close();
                } else {
                }
            }
        } else {
            pthread_exit(nullptr);
        }
    }
#endif
}

Database::Database* Game::Connection::getDatabase() const noexcept {
#ifdef SERVER
    return getOwner<Server::GameSession>()->getDatabase();
#else
    return nullptr;
#endif
}
