#ifndef LTYPE_GAME_SPAWNER_H
#define LTYPE_GAME_SPAWNER_H

#include "../Abstract/GameEntity.hpp"
#include "../Projectile.hpp"
#include "Level.hpp"
#include "../EnemyType/Swarmling.hpp"
#include "../Abstract/EntityType.hpp"
#include "../Connection.hpp"
#include "../GameState.hpp"
#include "../../Utils/Constants.hpp"
#include <iostream>
#include "../../Utils/Time.hpp"

namespace Game::Server {
    /**
     * Reponsible for reading level data from file .txt.
     * Keeps track of the Timer and spawns enemies when AdvanceInLevel is called,
     * at the position and time given by the Level
     */
    class Spawner {
        unsigned long _time_last_spawned = 0;
        unsigned long _offset            = 0;
        Utils::Timer  _timer;
        Level         _level;
        Connection* _connection;
        GameState * _game_state;

    protected:
        // needed to play the right boss music
        int getBossNumber(const EntityType boss_type);

    public:
        Spawner(Connection* connection, GameState* game_state,
                const std::string& path) :
            _timer(1) , _level(Level(path)),
            _connection(connection), _game_state(game_state) {
        }

        Spawner(const Spawner&) noexcept=delete;
        Spawner& operator=(const Spawner&) noexcept=delete;
		
		bool fileExists(const std::string& path) { return _level.fileExists(path); }
		
        bool setLevel(const std::string& path);
        // to create any entity in the game
        GameEntity* newEntity(EntityType type,
                              Vector2D position = Vector2D(
                                  static_cast<int>(WIDTH_WINDOW / 2), 0
                              ),
                              bool is_hostile = true,
                              PlayerShip* shooter = nullptr);
        // to create a new projectile, convert the ProjectileType to EntityType                      
        GameEntity* newEntity(ProjectileType type,
                              Vector2D position = Vector2D(
                                  static_cast<int>(WIDTH_WINDOW / 2), 0
                              ),
                              bool is_hostile = true,
                              PlayerShip* shooter = nullptr) {
        	return newEntity(Projectile::toEntityType(type), position, is_hostile, shooter);
        }
        // spawns all entities needed between now and the last time it was called
        bool advanceInLevel();

        ~Spawner() noexcept = default;

    };
}

#endif