#ifndef LTYPE_GAME_SHIP_PLAYERSHIP_H
#define LTYPE_GAME_SHIP_PLAYERSHIP_H

#include "Abstract/Ship.hpp"
#include "Abstract/GameEntity.hpp"
#include <iostream>
#include <string>
#include <ncurses.h>
#include "Abstract/EntityType.hpp"
#include <vector>

namespace Game {
    /**
     * A Ship that is controled by a player, 
     * destroys everything except Boss and PlayerShip when losing a _life
     * can pick up Bonus to change it's weapon/gain a life back,
     * and is only destroyed when it has no more _life.
     */
    class PlayerShip final: public Ship {
        int _score = 0;
        int _life  = 3;
    
    protected:
        void onDeath(GameInstance* instance);
        
    public:
        // PlayerShip() noexcept = default;

        PlayerShip(Game::EntityType type, std::size_t id, int w, int h,
                   Vector2D position, std::string style, CollisionMask mask,
                   int hp, ProjectileType projectile, bool is_hostile) :
            Ship(type, id, w, h, position, style, mask, hp, projectile,
                 is_hostile, "sprites/spaceship/Player/spaceship") {};

        PlayerShip(Game::EntityType type, std::size_t id,
                   Game::Vector2D position);
        inline PlayerShip(const PlayerShip&) noexcept = delete;
        inline PlayerShip(PlayerShip&& p) noexcept = delete;
        inline PlayerShip& operator=(const PlayerShip&) noexcept = delete;
        inline PlayerShip& operator=(PlayerShip&& p) noexcept = delete;

        inline int getScore() const { return _score; }

        inline int getLife() const { return _life; }

        inline void setScore(int s) { _score = s; }

        inline void setLife(int l) { _life = l; }

        void nuke(GameInstance* instance);

        inline void lostLife(int l) { _life -= l; }

        void moveToCenter();

        void draw(sf::RenderWindow* =nullptr) override;

        void onUpdate(GameInstance* instance = nullptr) override;
        
        void onCollision(Game::GameEntity* other,
                         GameInstance* instance = nullptr) override;

        void shoot(GameInstance* instance) override;

        static void increaseScore(GameInstance* instance,
                                  PlayerShip* shooter,
                                  double score_multiplier);

        ~PlayerShip() noexcept final=default;
    };
}

#endif // LTYPE_GAME_SHIP_PLAYERSHIP_H