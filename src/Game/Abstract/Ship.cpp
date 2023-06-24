#include "Ship.hpp"
#include "../Server/GameInstance.hpp"

void Game::Ship::onUpdate(GameInstance*) {
    #ifdef SERVER
    setPosition(getPosition() + _speed / 15);
    #else
    GameEntity::onUpdate();
    #endif
}

void Game::Ship::shoot(GameInstance* instance) {
    if(_shoot_cooldown.isTriggered(Projectile::getAttackSpeed(_projectile))) {
        Vector2D position = getPosition()+Vector2D(static_cast<float>(getWidth() / 2),
                                                   static_cast<float>(getHeight() * _is_hostile) + 
                                                   Projectile::getDimensions(getProjectile()).getY()/2);
        instance->getSpawner()->newEntity(_projectile, position, isHostile());
    }
}

Game::Ship::~Ship() = default;
