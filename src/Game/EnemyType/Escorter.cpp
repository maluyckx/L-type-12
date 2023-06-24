#include "Escorter.hpp"
#include "../Server/GameInstance.hpp"

const std::vector<Game::Vector2D> _movement_pattern_escorter = {
    Game::Vector2D::left(), Game::Vector2D::left() + Game::Vector2D::down(),
    Game::Vector2D::right(), Game::Vector2D::right()
};
const int                         _movement_speed_escorter   = 1;

Game::Escorter::Escorter(size_t id, Game::Vector2D pos,
                         int difficulty_modifier) :
    EnemyShip(EntityType::ShipEscorter,
              id,
              EntityDimensions::ShipEscorter_WIDTH,
              EntityDimensions::ShipEscorter_HEIGHT,
              pos,
              STYLE_ENEMYESCORTER,
              1 * difficulty_modifier,
              ProjectileType::Normal,
              PATH_ESCORTER,
              _movement_speed_escorter) {
    _loot = Lootable(EntityType::BonusWeaponLaser);
}

void Game::Escorter::movePattern() {
    if (_movement_timer.isTriggered()) {
        setSpeed(_movement_pattern_escorter[_move_pattern_location]);
        ++_move_pattern_location;
        if (_move_pattern_location == _movement_pattern_escorter.size())
            _move_pattern_location = 0;
    }
}

void Game::Escorter::shoot(GameInstance* instance) {
    if (_shoot_cooldown.isTriggered(Projectile::getAttackSpeed(_projectile),
                                    false)) {
        _shoot_cooldown.isTriggered(Projectile::getAttackSpeed(_projectile) * 1.5f);
        if (_cooldown_spray.isTriggered()) {
            Vector2D position = getPosition() +
                                Vector2D((static_cast<float>(getWidth()) / 2),
                                         static_cast<float>(getHeight()));
            instance->getSpawner()->newEntity(getProjectile(), position,
                                              _is_hostile);
        }
    }
}
