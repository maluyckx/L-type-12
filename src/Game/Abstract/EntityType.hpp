#ifndef LTYPE_GAME_ENTITYTYPE_H
#define LTYPE_GAME_ENTITYTYPE_H

#include "../../Utils/Constants.hpp"
#include "../Vector2D.hpp"

namespace Game {

    const Vector2D INITIAL_POSITION_P1 = Vector2D(WIDTH_WINDOW / 4, 30);
    const Vector2D INITIAL_POSITION_P2 = Vector2D(WIDTH_WINDOW / 4 * 3, 30);
	
    /**
     * The different kinds of projectiles, also found in EntityType.
     */
    enum class ProjectileType {
        Small,
        Normal,
        Big,
        Laser,
        Missile,
        Fire
    };

    /**
     * The Generic Types, corresponding to the mother classes, 
     * it is a simplified version of EntityType.
     */
    enum class GenericType {
        PlayerShip,
        EnemyShip,
        Boss,
        Bonus,
        Projectile,
        Obstacle,
        HUD
    };

    enum class EntityType : unsigned {
        ShipPlayer1        = 1 << 1,
        ShipPlayer2        = 1 << 2,
        ShipSwarmling      = 1 << 3,
        ShipBattleship     = 1 << 4,
        ShipLaserGun       = 1 << 5,
        ShipEscorter       = 1 << 6,
        ShipCruiser        = 1 << 7,
        BossAsteroidBase   = 1 << 8,
        BossEnterprise     = 1 << 9,
        BossDragon         = 1 << 10,
        BossPoulpe         = 1 << 11,
        BonusHeal          = 1 << 12,
        BonusWeaponMissile = 1 << 13,
        BonusWeaponLaser   = 1 << 14,
        BonusWeaponSmall   = 1 << 15,
        Asteroid           = 1 << 16,
        HUD                = 1 << 17,
        ProjectileSmall    = 1 << 18,
        ProjectileNormal   = 1 << 19,
        ProjectileBig      = 1 << 20,
        ProjectileLaser    = 1 << 21,
        ProjectileMissile  = 1 << 22,
        ProjectileFire     = 1 << 23,
        Star               = 1 << 24,
    };

    enum EntityDimensions {
        ShipPlayer1_WIDTH       = 7,
        ShipPlayer1_HEIGHT      = 3,
        EnemyShip_WIDTH         = 6,
        EnemyShip_HEIGHT        = 2,
        ShipSwarmling_WIDTH     = 3,
        ShipSwarmling_HEIGHT    = 1,
        ShipBattleship_WIDTH    = 7,
        ShipBattleship_HEIGHT   = 3,
        ShipLaserGun_WIDTH      = 3,
        ShipLaserGun_HEIGHT     = 2,
        ShipEscorter_WIDTH      = 3,
        ShipEscorter_HEIGHT     = 1,
        ShipCruiser_WIDTH       = 5,
        ShipCruiser_HEIGHT      = 1,
        BossAsteroidBase_WIDTH  = 25,
        BossAsteroidBase_HEIGHT = 13,
        BossEnterprise_WIDTH    = 43,
        BossEnterprise_HEIGHT   = 6,
        BossDragon_WIDTH        = 25,
        BossDragon_HEIGHT       = 15,
        BossPoulpe_WIDTH        = 30,
        BossPoulpe_HEIGHT       = 7,
        Bonus_WIDTH             = 2,
        Bonus_HEIGHT            = 1,
        ProjectileNormal_WIDTH  = 1,
        ProjectileNormal_HEIGHT = 2,
        ProjectileBig_WIDTH     = 2,
        ProjectileBig_HEIGHT    = 1,
        ProjectileLaser_WIDTH   = 2,
        ProjectileLaser_HEIGHT  = 1,
        ProjectileSmall_WIDTH   = 1,
        ProjectileSmall_HEIGHT  = 1,
        Fire_WIDTH              = 2,
        Fire_HEIGHT             = 3,
        Asteroid_WIDTH          = 3,
        Asteroid_HEIGHT         = 2,
        HUD_WIDTH               = WIDTH_WINDOW,
        HUD_HEIGHT              = 7,
        Star_WIDTH              = 1,
        Star_HEIGHT             = 1
    };
}

#endif
