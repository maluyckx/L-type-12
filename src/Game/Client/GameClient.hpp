#ifndef LTYPE_GAME_GAMECLIENT_H
#define LTYPE_GAME_GAMECLIENT_H

#include "../Abstract/AppInstance.hpp"
#include "../Connection.hpp"
#include "../Command.hpp"
#include "../GameState.hpp"
#include "Sound.hpp"

namespace Game::Client {
    class Window;

    /**
     * The main class for the client version of the game.
     * Links with the Menu system, hold the window and interacts with the factory/executor
     */
    class GameClient final : public AppInstance {
    private:
        Connection* _connection{};
        Window    * _window{};
        GameState * _state{};
        Sound* _sound{};
    public:
        GameClient(bool isGfx);
        GameClient(const GameClient& other) = delete;
        GameClient(GameClient&& other) noexcept;
        GameClient& operator=(const GameClient& rhs) = delete;
        GameClient& operator=(GameClient&& rhs) noexcept;
        ~GameClient() override; 
        void run() override;
        void quit();

        Window* getWindow() const { return _window; }

        Connection* getConnection() const { return _connection; }

        [[maybe_unused]]
        void pushOnSendingQueue(Command* command) {
            _connection->getSendingCommandQueue()->push(command);
        }


        GameState* getGameState() const noexcept { return _state; }

        void setGameState(GameState* state) { _state = state; }

        void startGame(char* username_player1, char*
        username_player2, Game::GameSettings settings);
        
        void endGame();
        Sound* getSound(){ return _sound;}

    private:
        /// asks the server for a new session
        unsigned _request_session_from_server(pid_t client_pid);

        constexpr bool isSessionBad() {
            return _connection->getSessionID() == BAD_SESSION;
        }
    };
}

#endif