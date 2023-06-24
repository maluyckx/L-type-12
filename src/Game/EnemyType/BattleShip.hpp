#ifndef LTYPE_GAME_SHIP_BATTLESHIP_H
#define LTYPE_GAME_SHIP_BATTLESHIP_H

#include "../Abstract/EnemyShip.hpp"

namespace Game {
    /**
     * Big EnemyShip that shoots big projectiles (mines),
     * comes down fast then slows down
     * and sometimes gives health bonus on death.
     */
    class BattleShip : public EnemyShip {
        
    public:
        BattleShip(size_t id, Vector2D pos, int difficulty_modifier);
        
		void movePattern() override;
        
        ~BattleShip() noexcept = default;
    };
}

#endif