#ifndef LTYPE_GAME_BOSS_SHREK_H
#define LTYPE_GAME_BOSS_SHREK_H

#include "../../Utils/Constants.hpp"
#include "../Abstract/EntityType.hpp"
#include "../Projectile.hpp"
#include "../Abstract/Boss.hpp"
#include "../Vector2D.hpp"
#include "../Lootable.hpp"
#include "../../Utils/Time.hpp"

namespace Game {
    /**
     * THE Boss that moves fast in all directions, 
     * bouncing when it hits the screen's edges
     * it shoots special Fire projectiles.
     */
    class Dragon final : public Boss {
        Utils::Timer _fire_timer = Utils::Timer();  // fire faster when low hp
        Utils::Timer *_animation_timer = nullptr;

    public:
        Dragon(long unsigned int id, int diff, Vector2D position);
        
        Dragon(const Dragon&) noexcept = delete;
        Dragon& operator=(const Dragon&) noexcept = delete;
        // used to make the client animate the dragon
        void fireAnimation();

        void onUpdate(GameInstance* instance=nullptr) override;
        
		void onCollision(GameEntity* other, GameInstance* instance) override;
		
        void shoot(GameInstance* instance);

        ~Dragon() noexcept = default;
    };
    

}

#endif // LTYPE_GAME_BOSS_SHREK_H