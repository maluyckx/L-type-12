#include "Swarmling.hpp"
#include "../Server/GameInstance.hpp"

const std::vector<Game::Vector2D> _movement_pattern = {
    Game::Vector2D(1, 1), Game::Vector2D(-1, 0.2f), Game::Vector2D(0, 1.5f)};
const int                         _movement_speed   = 1;

Game::Swarmling::Swarmling(size_t id, Game::Vector2D pos,
                           int difficulty_modifier) :
    EnemyShip(EntityType::ShipSwarmling,
              id,
              EntityDimensions::ShipSwarmling_WIDTH,
              EntityDimensions::ShipSwarmling_HEIGHT,
              pos,
              STYLE_ENEMYSWARMLING,
              difficulty_modifier,
              ProjectileType::Small,
              PATH_SWARMLING,
              _movement_speed) {}

void Game::Swarmling::movePattern() {
    if (_movement_timer.isTriggered()) {
        setSpeed(_movement_pattern[_move_pattern_location]);
        ++_move_pattern_location;
        if (_move_pattern_location ==
            _movement_pattern.size())
            _move_pattern_location = 0;
    }
}

void Game::Swarmling::shoot(GameInstance* instance) {
    if (_shoot_cooldown.isTriggered(Projectile::getAttackSpeed(_projectile) * 2)) {
        Vector2D position = getPosition() +
                            Vector2D((static_cast<float>(getWidth()) / 2),
                                     static_cast<float>(getHeight()));
        instance->getSpawner()->newEntity(getProjectile(), position,
                                          _is_hostile);
    }
}
