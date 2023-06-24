#ifndef LTYPE_GAME_BOSS_BOSSPOULPE_H
#define LTYPE_GAME_BOSS_BOSSPOULPE_H

#include "../Abstract/Boss.hpp"
#include "../../Utils/Constants.hpp"
#include "../Abstract/EntityType.hpp"
#include "../Projectile.hpp"
#include "../Abstract/Destructable.hpp"
#include "../../Utils/Time.hpp"

namespace Game {
    /**
     * Boss that moves from left to right
     * shooting big projectiles from the center
     * and bursts of lasers left and right.
     */
    class BossPoulpe final : public Boss {
        Utils::Timer _shoot_cooldown = Utils::Timer();
        Utils::Timer _cooldown_spray = Utils::Timer(Projectile::getAttackSpeed(ProjectileType::Laser) / 30);
        bool _shoot_left = true;

    public:
        BossPoulpe(long unsigned int id, int diff, Vector2D position);
        // shoots in spray with different weapons
        virtual void shoot(GameInstance* instance, ProjectileType type, int offset);

        virtual void onUpdate(GameInstance* instance = nullptr) override;

        virtual ~BossPoulpe() noexcept override = default;
    };

}

#endif // LTYPE_GAME_BOSS_BOSSPOULPE_H