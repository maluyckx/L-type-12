#ifndef LTYPE_GAME_SHIP_LASERGUN_H
#define LTYPE_GAME_SHIP_LASERGUN_H

#include "../Abstract/EnemyShip.hpp"

namespace Game {
    /**
     * Average size EnemyShip that goes in diagonal
     * shooting Lasers with average hp
     * gives laser on death.
     */
    class LaserGun : public EnemyShip {

    public:
        LaserGun(size_t id, Vector2D pos, int difficulty_modifier);

        void movePattern() override;
        
        ~LaserGun() noexcept = default;
    };
}


#endif