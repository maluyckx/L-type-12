#ifndef LTYPE_GAME_GAMESESSION_H
#define LTYPE_GAME_GAMESESSION_H

#include <unistd.h>
#include <pthread.h>
#include "../Connection.hpp"
#include <iostream>
#include "GameInstance.hpp"

namespace Game::Client {
    enum class DisplayMode; // forward declaration
}
namespace Game::Server {
    class GameServer;

    enum class GameSessionState {
        NotStarted,
        Running,
        Terminated,
    };

    /**
     * Represents a Session of a game running on the server.
     * It cointains data for the pre-game settings and a clock for the spawner.
     */
    class GameSession {
    private:
        /// unique id of the session, used for the connection
        unsigned         _id;
        GameSessionState _state;
        pthread_mutex_t* _state_mutex;
        GameServer     * _server;
        pthread_t      * _thread;
        Connection     * _connection;
        GameInstance   * _current_game = nullptr;
        Client::DisplayMode _display_mode;
    public:
        GameSession() noexcept = delete;
        GameSession(GameServer* server, unsigned session_id, pid_t client_pid,
                    Client::DisplayMode mode);
        GameSession(const GameSession& other) = delete;
        GameSession(GameSession&& other) noexcept;
        GameSession& operator=(const GameSession& rhs) = delete;
        GameSession& operator=(GameSession&& rhs) noexcept;
        ~GameSession() noexcept;
        bool start();
        static void* run(void* args);

        [[nodiscard]] GameSessionState getState() const {
            pthread_mutex_lock(_state_mutex);
            auto state = _state;
            pthread_mutex_unlock(_state_mutex);
            return state;
        }

        void setState(GameSessionState new_state) {
            pthread_mutex_lock(_state_mutex);
            _state = new_state;
            pthread_mutex_unlock(_state_mutex);
        }

        [[maybe_unused]]
        bool isDisplayTerminal() const;
        bool isDisplayGraphical() const;

        constexpr Connection* getConnection() const { return _connection; };

        Database::Database* getDatabase() const noexcept;

        void updateGameInstance() {
            if (_current_game) {
                if (_current_game->getGameState()->isOver()) {
                    endGame();
                } else {
                    _current_game->run();
                }
            }
        }

        GameInstance* getGameInstance() const noexcept { return _current_game; }

        void setGameInstance(GameInstance* game) { _current_game = game; }

        void startGame(char* username_player1,
                       char* username_player2,
                       Game::GameSettings settings);
        void endGame();
        bool isUserLoggedIn(int user_id);
        void logInUser(int user_id);
        void logOutUser(int user_id);
        void logOutAllUsers();
    };
}

#endif
