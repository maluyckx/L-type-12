#include "Obstacle.hpp"
#include "../Client/Sound.hpp"
#include "../Command.hpp"

Game::Obstacle::Obstacle(std::size_t id, Game::Vector2D position) : 
    Destructable(EntityType::Asteroid, id, 
                 EntityDimensions::Asteroid_WIDTH, 
                 EntityDimensions::Asteroid_HEIGHT, 
                 position, 
                 STYLE_OBSTACLE, 
                 CollisionMask::OBSTACLE, 
                 2, 
                 PATH_OBSTACLE+std::to_string(id%13+5)) {}
        

 void Game::Obstacle::draw(sf::RenderWindow* window) {
    if(window) {
        GameEntity::pencil(*this, window);
    } else {
        GameEntity::pencil(*this,
            TERMINAL_COLORS::WHITE);
    }
}

void Game::Obstacle::onUpdate(Game::GameInstance*) {
	#ifdef SERVER
	setPosition(getPosition()+getSpeed()/5);
	#else
	GameEntity::onUpdate();
	#endif
}

void Game::Obstacle::onCollision(Game::GameEntity* other,
                                 GameInstance* instance) {
    if (!isDestroyed()) {
        if (other) {
            switch (GameEntity::getGenericType(other->getType())) {
                case GenericType::HUD: {
                    _collided_with_hud = true;
                    break;
                }
                case GenericType::Projectile: {
                    auto projectile = dynamic_cast<Projectile*>(other);
                    lostHp(Projectile::getDamage(
                            projectile->getProjectileType()));
                    break;
                }
                default:
                    lostHp(1);
                    break;
            }
        } else {
            // we hit a wall
            if (_collided_with_hud) {
                destroy();
            }
        }
        if (!isAlive()) {
            destroy();
            instance->getConnection()->getSendingCommandQueue()->push(
                    CommandFactory::playSound(Game::Client::SoundEffect::HitRockSound));
            _loot.spawnBonus(instance, getPosition());
        }
    }
}
