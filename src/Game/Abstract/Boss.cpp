#include "Boss.hpp"
#include "../Vector2D.hpp"
#include "../Command.hpp"
#include "../Client/SFML/ParticleHandler.hpp"
#include "../Server/GameSession.hpp"
#include "../Server/GameInstance.hpp"

Game::Boss::Boss(Game::EntityType type, std::size_t id, int w, int h,
                 Game::Vector2D pos, std::string style, int hp, std::string path):
                    Destructable(type, id, w, h, pos, style, CollisionMask::EMPIRE, hp, std::move(path)) {}

void Game::Boss::draw(sf::RenderWindow* window) {
    if(window) {
        GameEntity::pencil(*this, window);
    } else {
        GameEntity::pencil(*this,
            TERMINAL_COLORS::WHITE);
    }
}

#ifdef SERVER
void Game::Boss::onUpdate(Game::GameInstance*) {
    setPosition(getPosition()+getSpeed()/20);
}
#else
void Game::Boss::onUpdate(Game::GameInstance*) {
    GameEntity::onUpdate();
}
#endif

void Game::Boss::onCollision(Game::GameEntity* other, GameInstance* instance) {
    if (!isDestroyed()) {
        if (other) {
            switch (GameEntity::getGenericType(other->getType())) {
                case GenericType::Projectile: {
                	if(getY()>=0) {
		                auto projectile = dynamic_cast<Projectile*>(other);
		                if (!projectile->isHostile()) {
		                    lostHp(Projectile::getDamage(
		                        projectile->getProjectileType()));
		                    if (!isAlive()) {
		                        PlayerShip::increaseScore(instance,
		                                                  projectile->getShooter(),
		                                                  300);
		                    }
		                }
		            }
                    break;
                }
                case GenericType::HUD: {
                	break;
                }
                default:
                    break;
            }
        }
        if (!isAlive()) {
            destroy();
            _loot.spawnBonus(instance, getPosition() + Vector2D(static_cast<float>(getWidth()/2)-2.f, static_cast<float>(getHeight()/2)));
            instance->getGameState()->getHUD()->onUpdate(instance);
            if(instance->getConnection()->getOwner<Game::Server::GameSession>()->isDisplayGraphical()) {
		        instance->getConnection()->getSendingCommandQueue()->push(
		            CommandFactory::makeParticle(SFML::Particle::MassiveExplosion,
		                                         getX(),
		                                         getY(),
		                                         static_cast<float>(getWidth()),
		                                         static_cast<float>(getWidth()),
                                                 false)
		        );
		    }
        }
    }
}