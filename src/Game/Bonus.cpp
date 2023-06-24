#include "Bonus.hpp"
#include "Client/Sound.hpp"
#include "Command.hpp"
#include "Client/SFML/ParticleHandler.hpp"
#include "Server/GameSession.hpp"

Game::Bonus::Bonus(EntityType type, size_t id, Vector2D pos) : 
    GameEntity(type, id, pos) {
        if(type==EntityType::BonusWeaponMissile) {
            proj_type = ProjectileType::Missile;
        } else if(type==EntityType::BonusWeaponLaser) {
            proj_type = ProjectileType::Laser;
        } else if(type==EntityType::BonusWeaponSmall) {
            proj_type = ProjectileType::Small;
        }
    }

void Game::Bonus::draw(sf::RenderWindow* window) {
    if(window) {
        GameEntity::pencil(*this, window);
    } else {
        GameEntity::pencil(*this,
            TERMINAL_COLORS::GREEN);
    }
}

void Game::Bonus::onUpdate(GameInstance*) {
	#ifdef SERVER
    setPosition(getPosition()+getSpeed()/30);
    #else
    GameEntity::onUpdate();
    #endif
}

void Game::Bonus::onCollision(Game::GameEntity* other, GameInstance* instance) {
    if(!isDestroyed()) {
        if(other) {
            if(GameEntity::getGenericType(other->getType())==GenericType::PlayerShip) {
                instance->getConnection()->getSendingCommandQueue()->push(
                        CommandFactory::playSound(Game::Client::SoundEffect::BonusRecovery));   
                if(instance->getConnection()->getOwner<Game::Server::GameSession>()->isDisplayGraphical()) {
                    SFML::Particle bonustype;
                    if(getType()==EntityType::BonusHeal)
                        bonustype = SFML::Particle::BonusHealRecovered;
                    else
                        bonustype = SFML::Particle::BonusWeaponRecovered;
                    instance->getConnection()->getSendingCommandQueue()->push(
                        CommandFactory::makeParticle(bonustype,
                                                    getX()-EntityDimensions::Bonus_WIDTH,
                                                    getY()-EntityDimensions::Bonus_HEIGHT,
                                                    6,
                                                    6,
                                                    false)
                    );
                }
                destroy();
            }
        } else { destroy(); }  // wall
    }
}
