#ifndef LTYPE_GAME_SHIP_ESCORTER_H
#define LTYPE_GAME_SHIP_ESCORTER_H

#include "../Abstract/EnemyShip.hpp"
#include "../../Utils/Time.hpp"

namespace Game {
    /**
     * Small elite EnemyShip that moves from left to right while sometimes going down,
     * shooting bursts of Normal projectiles 
     * and giving Laser on death.
     */
    class Escorter : public EnemyShip {
        Utils::Timer _cooldown_spray = Utils::Timer(Projectile::getAttackSpeed(ProjectileType::Normal)/6);

    public:
        Escorter(size_t id, Vector2D pos, int difficulty_modifier);
        
        void movePattern() override;
        // shoots in spray
        void shoot(GameInstance* instance) override;

        ~Escorter() noexcept = default;
    };
}


#endif