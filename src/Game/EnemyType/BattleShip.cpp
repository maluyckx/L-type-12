#include "BattleShip.hpp"
#include "../Server/GameInstance.hpp"

const std::vector<Game::Vector2D> _movement_pattern_battleship = {
    Game::Vector2D::down() * 0.5f
};
const int                         _movement_speed_battleship   = 5;

Game::BattleShip::BattleShip(size_t id, Game::Vector2D pos,
                             int difficulty_modifier) :
    EnemyShip(EntityType::ShipBattleship,
              id,
              EntityDimensions::ShipBattleship_WIDTH,
              EntityDimensions::ShipBattleship_HEIGHT,
              pos,
              STYLE_ENEMYBATTLESHIP,
              3 * difficulty_modifier,
              ProjectileType::Big,
              PATH_BATTLESHIP,
              _movement_speed_battleship) {
    _loot = Lootable(EntityType::BonusHeal);
}

void Game::BattleShip::movePattern() {
    if (_movement_timer.isTriggered()) {
        setSpeed(_movement_pattern_battleship[_move_pattern_location]);
    }
}
