#include "EnemyShip.hpp"
#include "../../Utils/DebugLogger.hpp"
#include "../PlayerShip.hpp"
#include "../Server/GameInstance.hpp"
#include "../Client/Sound.hpp"
#include "../Command.hpp"
#include "../Server/GameSession.hpp"

Game::EnemyShip::EnemyShip(EntityType type, std::size_t id, int width, int height,
                           Vector2D position, std::string style, int hp,
                           ProjectileType projectile, std::string path, 
                           int movement_speed) :
                              Ship(type, id, width, height, position, std::move(style),
                                   CollisionMask::EMPIRE, hp, projectile, true, std::move(path)
                                   ), _movement_timer(movement_speed) {}

void Game::EnemyShip::draw(sf::RenderWindow* window) {
    if(window) {
        GameEntity::pencil(*this, window);
    } else {
        GameEntity::pencil(*this,
            TERMINAL_COLORS::YELLOW);
    }
}

void Game::EnemyShip::onUpdate(Game::GameInstance* instance) {
    Game::Ship::onUpdate(instance);
#ifdef SERVER
    movePattern();
    shoot(instance);
#else
#endif
}

void Game::EnemyShip::onCollision(Game::GameEntity* other,
                                  Game::GameInstance* instance) {
    if (!isDestroyed()) {
        if (other) {
            switch (GameEntity::getGenericType(other->getType())) {
                case GenericType::PlayerShip: {
                    setHp(0);
                    break;
                }
                case GenericType::Obstacle: {
                    lostHp(1);
                    break;
                }
                case GenericType::HUD: {
                    _collided_with_hud = true;
                    break;
                }
                case GenericType::Projectile: {
                    auto projectile = dynamic_cast<Projectile*>(other);
                    if (!projectile->isHostile()) {
                        lostHp(Projectile::getDamage(
                            projectile->getProjectileType()));
                        if (!isAlive()) {
                            PlayerShip::increaseScore(instance,
                                                      projectile->getShooter(),
                                                      100);
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        } else {
            // we hit a wall
            if (_collided_with_hud) {
                destroy();
            }
        }
        if (!isAlive()) {
            _loot.spawnBonus(instance, getPosition());
            instance->getConnection()->getSendingCommandQueue()->push(
                    CommandFactory::playSound(Game::Client::SoundEffect::HitEnemySound));
            if(instance->getConnection()->getOwner<Game::Server::GameSession>()->isDisplayGraphical()) {
		        float radius = static_cast<float>(getWidth()) * static_cast<float>(getHeight());
		        if(getType()==EntityType::ShipBattleship)
		        	radius*=0.5f;
		        instance->getConnection()->getSendingCommandQueue()->push(
		            CommandFactory::makeParticle(SFML::Particle::ShipExplosion,
		                                        getX(),
		                                        getY(),
		                                        radius,
		                                        radius)
		        );
		    }
            destroy();
        }
    }
}

