#include "Lootable.hpp"
#include <cstdlib>
#include "Server/GameInstance.hpp"


void Game::Lootable::spawnBonus(GameInstance* instance, Vector2D position) {
    if(is_garanteed || (std::rand() % 10) < instance->getGameState()->getGameSettings().getDropRate())
        instance->getSpawner()->newEntity(_type, position);
}