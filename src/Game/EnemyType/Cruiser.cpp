#include "Cruiser.hpp"
#include "../Server/GameInstance.hpp"

const std::vector<Game::Vector2D> _movement_pattern_cruiser = {
    Game::Vector2D::left(), Game::Vector2D::down() * 1.2f,
    Game::Vector2D::right()};
const int                         _movement_speed_cruiser   = 3;

Game::Cruiser::Cruiser(size_t id, Game::Vector2D pos, int difficulty_modifier) :
    EnemyShip(EntityType::ShipCruiser, id,
              EntityDimensions::ShipCruiser_WIDTH,
              EntityDimensions::ShipCruiser_HEIGHT,
              pos,
              STYLE_ENEMYCRUISER,
              2 * difficulty_modifier,
              ProjectileType::Missile,
              PATH_CRUISER,
              _movement_speed_cruiser) {
    _loot = Lootable(EntityType::BonusWeaponMissile);
}

void Game::Cruiser::movePattern() {
    if (_movement_timer.isTriggered()) {
        setSpeed(_movement_pattern_cruiser[_move_pattern_location]);
        ++_move_pattern_location;
        if (_move_pattern_location == _movement_pattern_cruiser.size())
            _move_pattern_location = 0;
    }
}
