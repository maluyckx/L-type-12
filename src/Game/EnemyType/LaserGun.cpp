#include "LaserGun.hpp"
#include "../Server/GameInstance.hpp"

const std::vector<Game::Vector2D> _movement_pattern_lasergun = {Game::Vector2D::down()+Game::Vector2D::right()};

Game::LaserGun::LaserGun(size_t id, Vector2D pos, int difficulty_modifier) :
    EnemyShip(EntityType::ShipLaserGun, 
                id,
                EntityDimensions::ShipLaserGun_WIDTH,
                EntityDimensions::ShipLaserGun_HEIGHT,
                pos, STYLE_ENEMYLASERGUN, 
                2 * difficulty_modifier,
                ProjectileType::Laser, 
                PATH_LASERGUN,
                1) {
        _loot = Lootable(EntityType::BonusWeaponLaser);
    }

void Game::LaserGun::movePattern() {
    setSpeed(_movement_pattern_lasergun[_move_pattern_location]);   
}