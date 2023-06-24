#include "Enterprise.hpp"
#include "../Abstract/Destructable.hpp"
#include "../Server/GameInstance.hpp"
#include "../Client/SFML/ParticleHandler.hpp"
#include "../Command.hpp"
#include "../Server/GameSession.hpp"

Game::Enterprise::Enterprise(long unsigned int id, int diff, Vector2D position) : 
    Boss(EntityType::BossEnterprise, id,
            EntityDimensions::BossEnterprise_WIDTH,
            EntityDimensions::BossEnterprise_HEIGHT,
            position, STYLE_BOSS_ENTERPRISE, 15*diff,
            PATH_ENTERPRISE) {}

void Game::Enterprise::shoot(GameInstance* instance) {
    if(_shoot_cooldown.isTriggered(Projectile::getAttackSpeed(_projectile)*0.3f,
                                false)) {
        _shoot_cooldown.isTriggered(Projectile::getAttackSpeed(_projectile)*0.6f);  // time to stop shooting
        if(_cooldown_spray.isTriggered()) {
            Vector2D position = getPosition()+Vector2D((static_cast<float>(getWidth()) / 5*4)+4,
                                                    static_cast<float>(getHeight()/2-0.75));
            instance->getSpawner()->newEntity(_projectile, position, true);
        }
    }
}

#ifdef SERVER
void Game::Enterprise::onUpdate(Game::GameInstance* instance) {
    if(_warp_cooldown.isTriggered()) {  // same timer used for both phase, repairs and teleportations
        int difficulty = instance->getGameState()->getGameSettings().getDifficulty();
        if(getHp() > difficulty*10) {  // first phase : repairs itself
            if(getHp() < difficulty*15) {
                setHp(getHp()+1);
                instance->getGameState()->getHUD()->onUpdate(instance);
            }
        } else {  // second phase : teleports
        	Vector2D new_pos = Vector2D(static_cast<float>(std::rand() % (WIDTH_WINDOW-getWidth())), 
                                        static_cast<float>(std::rand() % getHeight()));
        	while(new_pos.getX()-getX() < static_cast<float>(getWidth()/2) && getX()-new_pos.getX() < static_cast<float>(getWidth()/2)) {
            	new_pos.setX(static_cast<float>(std::rand() % (WIDTH_WINDOW-getWidth())));
        	} setPosition(new_pos);  // so it doesn't teleport too close to old position
            if(instance->getConnection()->getOwner<Game::Server::GameSession>()->isDisplayGraphical()) {
                instance->getConnection()->getSendingCommandQueue()->push(
                CommandFactory::makeParticle(SFML::Particle::Warp, 
                                             getX()-2.f,
                                             getY()-0.5f,
                                             static_cast<float>(getWidth()/2+4),
                                             static_cast<float>(getHeight())+1.f,
                                             false));
            }
        }
    }
    shoot(instance);
}
#else
void Game::Enterprise::onUpdate(Game::GameInstance*) {
    GameEntity::onUpdate();
}
#endif