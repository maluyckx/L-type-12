#include "Dragon.hpp"
#include "../Server/GameInstance.hpp"
#include "../Abstract/GameEntity.hpp"
#include "../Command.hpp"
#include "../Client/SFML/ParticleHandler.hpp"
#include "../Server/GameSession.hpp"
#include "../Abstract/EntityType.hpp"

Game::Dragon::Dragon(long unsigned int id, int diff, Vector2D position) : 
    Boss(EntityType::BossDragon, id,
         EntityDimensions::BossDragon_WIDTH,
         EntityDimensions::BossDragon_HEIGHT,
         position, STYLE_BOSS_DRAGON, 15*diff, 
         PATH_DRAGON) {}

void Game::Dragon::shoot(Game::GameInstance* instance) {
    double attack_speed = getHp()*4.5/(15*instance->getGameState()->getGameSettings().getDifficulty());
    if(attack_speed < 1)
        attack_speed = 1;
    // shoots faster when it is more damaged
    if(_fire_timer.isTriggered(attack_speed)) {
        Vector2D position = getPosition()+Vector2D((static_cast<float>(getWidth()) / 2)-EntityDimensions::Fire_WIDTH-static_cast<float>(getWidth()/3),
                                                   static_cast<float>(getHeight())+EntityDimensions::Fire_HEIGHT-static_cast<float>(getHeight()/4*3));
        instance->getSpawner()->newEntity(ProjectileType::Fire, position, true);
        if(instance->getConnection()->getOwner<Game::Server::GameSession>()->isDisplayGraphical()) {
            instance->getConnection()->getSendingCommandQueue()->push(
               CommandFactory::makeParticle(SFML::Particle::DragonFire));  // animate the dragon
        }
    }
}

void Game::Dragon::fireAnimation() {
    _animation_timer = new Utils::Timer(0.5);
    getSprite()->animate(1);
}

#ifdef SERVER
void Game::Dragon::onUpdate(Game::GameInstance* instance) {
    shoot(instance);
    Vector2D speed = getSpeed();
    if(getY() >= HEIGHT_WINDOW-static_cast<float>(getHeight())-9.f && speed.getY() == Vector2D::down().getY()) {
    	speed.setY(Vector2D::up().getY());
    	if(!speed.getX()) 
    		speed.setX(Vector2D::left().getX());
    } else if (getY() < 0 && speed.getY() == Vector2D::up().getY()) {
    	speed.setY(Vector2D::down().getY());
    } else if(getX() > WIDTH_WINDOW-static_cast<float>(getWidth())-1.f && speed.getX() == Vector2D::right().getX()) {
    	speed.setX(Vector2D::left().getX());
    } else if (getX() < 0 && speed.getX() == Vector2D::left().getX()) {
    	speed.setX(Vector2D::right().getX());
    }
    setSpeed(speed);
    setPosition(getPosition()+speed/7);
}
#else
void Game::Dragon::onUpdate(Game::GameInstance*) {
    if(_animation_timer && _animation_timer->isTriggered()) {
        delete _animation_timer;
        _animation_timer = nullptr;
        getSprite()->animate(1);
    }
    GameEntity::onUpdate();
}
#endif

void Game::Dragon::onCollision(Game::GameEntity* other, Game::GameInstance* instance) {
    if (!isDestroyed()) {
        if (other && (GameEntity::getGenericType(other->getType()) == GenericType::PlayerShip) && getY() > 0 ) {
        	Vector2D speed = getSpeed();
        	speed.setY(Vector2D::up().getY());
        	setSpeed(speed);
        	float spawn_point = 29.f-static_cast<float>(getHeight());
        	if(getY() > spawn_point)
            	setPosition(Vector2D(getX(),spawn_point-1.f));  // when it collides with the player makes sure there is no spawnkilling
        }
    } Boss::onCollision(other, instance);
}