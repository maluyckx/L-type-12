#ifndef LTYPE_GAME_GAMEINSTANCE_H
#define LTYPE_GAME_GAMEINSTANCE_H

#include "../Abstract/AppInstance.hpp"
#include "../GameSettings.hpp"
#include "../HUD.hpp"
#include "Spawner.hpp"
#include "../GameState.hpp"
#include "../Connection.hpp"
#include "../Input.hpp"
#include "../../Utils/Time.hpp"

namespace Game {
    /**
     * An Instance of a game, contains all data related to the game logique
     */
    class GameInstance {
        GameState      * _game_state;
        Server::Spawner* _spawner;
        Connection     * _connection;
        Input          * _input;
        bool        _started        = false;
        Utils::Timer* _slow_end     = nullptr;
        unsigned    _campaign_level = 0;

    public:
        GameInstance(char* username_player1, char* username_player2,
                     GameSettings settings, Connection* connection);
        GameInstance(const Game::GameInstance&) noexcept = delete;
        GameInstance& operator=(const Game::GameInstance&) noexcept = delete;
        /// Update local input with the input from client
        void updateInput(char* inputs);
        /// Reset the local input table/memory
        void clearInput();
        void start();
        /// The main loop for the game
        void run();
        /// Checks if the level ended
        void levelEnded();
        /// Do some pre-ending computes and end the game
        void lost();
        /**
         * End the game and notify the client
         * @param quitting: did we quit the game from the pause menu
         * @param level_remain: the number of campaign level remaining
         */
        void end(bool quitting, int level_remain);
        /// Sends entities positions to the client
        void sendPositionsToClient();
        /// Sends new HUD data to the client
        void updateClientHUD();
        /// Handle inputs for the pause menu
        void handlePauseInputs();
        /**
         * The Garbage Collection for the entities that have been destroyed
         * this frame.
         * Why don't we delete entites directly?
         *  - To not have segfaults when we are still manipulating the
         *  entities, for exemple in a collision pair, one entity may collide
         *  with multiple entities, so we can't destroy it on the first
         *  "destruction". This is why we just set a flag up and destroy all
         *  "destroyed" entities at the end of the frame.
         */
        void clearDestroyedEntities();
        /// Destroy all the projectiles of a shooter we are destroying
        void clearProjectilesShooters(PlayerShip* shooter);
        /// Checks if players are still alive
        bool arePlayersAlive();

        GameState* getGameState() const { return _game_state; }

        Server::Spawner* getSpawner() const { return _spawner; }

        Connection* getConnection() const { return _connection; }

        /// Runs the physics thick of the game simulation
        void physicsUpdate(GameInstance* instance);

        Input* getInput() { return _input; }

        ~GameInstance() noexcept;
    };
}

#endif
