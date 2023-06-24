#ifndef LTYPE_GAME_BOSS_ENTERPRISE_H
#define LTYPE_GAME_BOSS_ENTERPRISE_H

#include "../Lootable.hpp"
#include "../../Utils/Constants.hpp"
#include "../Abstract/EntityType.hpp"
#include "../Projectile.hpp"
#include "../Abstract/Boss.hpp"
#include "../../Utils/Time.hpp"
#include "stdlib.h"


namespace Game {
	/**
     * Boss that regenerates it's hp if it isn't too damaged
     * or teleports at a random location
     * and shoots bursts of lasers anyway.
	 */
    class Enterprise final : public Boss {
        Utils::Timer _warp_cooldown = Utils::Timer(1.6);
        Utils::Timer _shoot_cooldown = Utils::Timer();
        unsigned int _spray_number = 1;
        ProjectileType _projectile = ProjectileType::Laser;
        Utils::Timer _cooldown_spray = Utils::Timer(Projectile::getAttackSpeed(_projectile)/30);

    public:
		Enterprise(long unsigned int id, int diff, Vector2D position);

        void onUpdate(GameInstance* instance=nullptr) override;
        
        void shoot(GameInstance* instance);

        ~Enterprise() noexcept = default;
    };

}

#endif // LTYPE_GAME_BOSS_ENTERPRISE_H