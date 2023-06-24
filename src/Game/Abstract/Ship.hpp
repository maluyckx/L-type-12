#ifndef LTYPE_GAME_SHIP_H
#define LTYPE_GAME_SHIP_H

#include <utility>

#include "Destructable.hpp"
#include "../Projectile.hpp"
#include "../../Utils/Time.hpp"

namespace Game {
    /**
     * A Destructable Entity that can shoot and move.
     */
    class Ship : public Destructable { // Abstract

    protected:        
        ProjectileType _projectile=ProjectileType::Normal;
        bool _is_hostile=true;
        Utils::Timer _shoot_cooldown = Utils::Timer();
        
        Ship() noexcept = default;
        Ship(Game::EntityType type, std::size_t id, int w, int h, Vector2D position, 
        std::string style, CollisionMask mask, int hp, ProjectileType projectile, bool is_hostile, std::string path) : 
            Destructable(type, id, w, h, position, std::move(style), mask, hp, std::move(path)), _projectile(projectile), _is_hostile(is_hostile) {};
    
    public:
        inline ProjectileType getProjectile() const { return _projectile; }

        inline void setProjectile(ProjectileType projectile) { _projectile = projectile; }

        //Will shoot a projectile from the current position of the ship. 
        virtual void shoot(GameInstance* instance);

        void onUpdate(GameInstance* instance=nullptr) override;
        //Returns if the ship is in the team of the player.
        bool isHostile() { return _is_hostile; }

        ~Ship() override = 0;
    };
    
}

#endif