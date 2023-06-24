#ifndef LTYPE_GAME_SHIP_ENEMYSHIP_H
#define LTYPE_GAME_SHIP_ENEMYSHIP_H

#include "Ship.hpp"
#include "../Lootable.hpp"
#include "EntityType.hpp"
#include <iostream>
#include <vector>

namespace Game{
    /**
     * Ship that moves and attacks the PlayerShip,
     * sometimes giving a bonus on death.
     */
    class EnemyShip : public Ship {
        bool _collided_with_hud = false;

    protected:
        Lootable     _loot                   = Lootable(EntityType::BonusWeaponSmall);  // what may be dropped on death
        unsigned long _move_pattern_location = 0;  //the place where we are in the vector of Vector2D representing deplacements
        Utils::Timer _movement_timer;

        EnemyShip(EntityType type, std::size_t id, int width, int height,
                  Vector2D position, std::string style, int hp,
                  ProjectileType projectile, std::string path, int
                  movement_speed);

    public:
        // change the direction (speed) of the ship everytime the chrono is reached.
        virtual void movePattern() = 0;

        void draw(sf::RenderWindow* =nullptr) override;

        void onUpdate(GameInstance* instance = nullptr) override;

        void onCollision(Game::GameEntity* other,
                         GameInstance* instance = nullptr) override;

        virtual ~EnemyShip() noexcept = default;
    };

}

#endif // LTYPE_GAME_SHIP_ENEMYSHIP_H
