#ifndef LTYPE_GAME_CONNECTION_H
#define LTYPE_GAME_CONNECTION_H

#include <pthread.h>
#include <unistd.h>
#include "../Utils/ThreadSafeQueue.hpp"
#include "../Database/Database.hpp"

namespace Game {
    struct Command;
    enum class ConnectionState {
        Open,
        Closed,
    };

    /**
     * Responsible for the connection between a client and a server.
     * It represents a bidirection named pipe.
     * It has 2 queues (1 for each direction) of commands that the CommandFactory can push to
     * and the CommandExecutor can pop from.
     * The general flow is (for the Client side):
     * 1) Inputs are read from the Window
     * 2) The GameClient sends the input to the CommandFactory
     * 3) The CommandFactory create a new Command based on the data and pushes to the "sending" queue of the connection
     * 4) The Connection pops from the sending queue and writes the command to the "sending" pipe
     * 5) The Connection reads from the "incoming" pipe and pushes the command to the "incoming" queue
     * 6) The CommandExecutor pops from the "incoming" queue and executes whatever the command is about
     *  (ex: updating Vector2DServer of a particular entity)
     * The flow on the server side is (remember connection on the client and on the server are 2 distinct
     *  instances living in the 2 diff processes):
     * 1) The Connection reads from the "incoming" pipe and pushes the command to the "incoming" queue
     * 2) The CommandExecutor pops from the "incoming" queue and executes whatever the command is about
     *  (usually will be a form of input from the player)
     * 3) An update() of the game will occur (not responsibility of connection)
     * 4) The Collision resolver will resolve collisions and save all the collision events
     *  (pair of entities that collided)
     * 5) The GameServer will take this list, and create a bunch of commands to update the positions of the entities and
     *  package the collision pairs, so the CommandFactory will push onto the "sending" queue.
     * 6) The Connection will pop the "sending" queue and write to the pipe.
     * Creates 2 named pipes using the pid+id of the sessions
     * TODO: will have think of a way to notify the client that the server created the pipe, a signal?
     *  use kill(pid, SIGCONT/SIGSTOP)
     */
    class Connection {
    private:
        void* _owner;
        ConnectionState _state;
        pthread_mutex_t* _state_mutex;
        unsigned int _session_id;
        pid_t        _client_pid;
        int          _client_to_session = 0;
        int          _session_to_client = 0;
        pthread_t* _reading_thread              = new pthread_t();
        pthread_t* _writing_thread              = new pthread_t();
        char     * _client_to_session_pipe_path = new char[50];
        char     * _session_to_client_pipe_path = new char[50];
        Utils::ThreadSafeQueue<Command*>* _sending_command_queue =
                                            new Utils::ThreadSafeQueue<Command*>();
        Utils::ThreadSafeQueue<Command*>* _receive_command_queue =
                                            new Utils::ThreadSafeQueue<Command*>();
#ifdef SERVER
#else
#endif
    public:
        Connection() = delete;
        Connection(void* owner, unsigned session_id, pid_t client_pid);
        Connection(const Connection& other) = delete;
        Connection(Connection&& other) noexcept;
        Connection& operator=(const Connection& rhs) = delete;
        Connection& operator=(Connection&& rhs) noexcept;
        ~Connection();
        bool open();
        bool close();

        [[nodiscard]] Utils::ThreadSafeQueue<Command*>*
        getSendingCommandQueue() const {
            return _sending_command_queue;
        }

        [[nodiscard]] Utils::ThreadSafeQueue<Command*>*
        getReceivingCommandQueue() const {
            return _receive_command_queue;
        }

        [[nodiscard]] char* getClientToSessionPipePath() const {
            return _client_to_session_pipe_path;
        }

        [[nodiscard]] char* getSessionToClientPipePath() const {
            return _session_to_client_pipe_path;
        }

        [[nodiscard]] constexpr unsigned getSessionID() const {
            return _session_id;
        }

        [[nodiscard]] constexpr pid_t getClientPID() const {
            return _client_pid;
        }

        ConnectionState getState() const {
            pthread_mutex_lock(_state_mutex);
            auto state = _state;
            pthread_mutex_unlock(_state_mutex);
            return state;
        }

        void setState(ConnectionState new_state) {
            pthread_mutex_lock(_state_mutex);
            _state = new_state;
            pthread_mutex_unlock(_state_mutex);
        }

        template<typename T>
        constexpr T* getOwner() const {
            return reinterpret_cast<T*>(_owner);
        }

        Database::Database* getDatabase() const noexcept;
        static void* send(void* args);
        static void* receive(void* args);
    };
}

#endif
