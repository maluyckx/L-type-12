#ifndef LTYPE_GAME_SHIP_SWARMLING_H
#define LTYPE_GAME_SHIP_SWARMLING_H

#include "../Abstract/EnemyShip.hpp"

namespace Game {
    /**
     * Small EnemyShip that is mighty annoying going in all directions
     * it shoots Small fast projectiles which it gives on death.
     */
    class Swarmling : public EnemyShip {
        
    public:
        Swarmling(size_t id, Vector2D pos, int difficulty_modifier);
		
		void movePattern() override;
        // shoots slower than the projectile attack speed so it isn't too strong
		void shoot(GameInstance* instance) override;

        ~Swarmling() noexcept = default;
    };
}


#endif