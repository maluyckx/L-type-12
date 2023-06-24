#ifndef LTYPE_GAME_SHIP_CRUISER_H
#define LTYPE_GAME_SHIP_CRUISER_H

#include "../Abstract/EnemyShip.hpp"

namespace Game {
    /**
     * Average EnemyShip that moves left, then down then right
     * shooting missiles which it gives on death.
     */
    class Cruiser : public EnemyShip {

    public:
        Cruiser(size_t id, Vector2D pos, int difficulty_modifier);

		void movePattern() override;
        
        ~Cruiser() noexcept = default;
    };
}


#endif