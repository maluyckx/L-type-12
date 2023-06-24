#include "AsteroidBase.hpp"
#include "../Server/GameInstance.hpp"

Game::AsteroidBase::AsteroidBase(long unsigned int id, int diff, Vector2D position) : 
    Boss(EntityType::BossAsteroidBase, id,
         EntityDimensions::BossAsteroidBase_WIDTH,
         EntityDimensions::BossAsteroidBase_HEIGHT,
         position, STYLE_BOSS_ASTEROIDBASE, 
         12*diff, 
         PATH_ASTEROID_BASE_CLOSE) {}

void Game::AsteroidBase::draw(sf::RenderWindow* window) {
    // the y position is used as a bool, slighty changed to know which appearance to have
    if(getY() && _is_spawning) {
    	if(window) 
            setSprite(PATH_ASTEROID_BASE_CLOSE);
    	else
            setStyle(STYLE_BOSS_ASTEROIDBASEOPEN);
        _is_spawning = false;
    } else if (!getY() && !_is_spawning){
        if(window) 
            setSprite(PATH_ASTEROID_BASE_OPEN);
        else
            setStyle(STYLE_BOSS_ASTEROIDBASE);
        _is_spawning = true;
    }
    if(window)
        GameEntity::pencil(*this, window);
    else {
        if(_is_spawning)
            GameEntity::pencil(*this, TERMINAL_COLORS::WHITE);
        else
            GameEntity::pencil(*this, TERMINAL_COLORS::CYAN);  // shields are up it is invincible
    }
}

void Game::AsteroidBase::spawn(GameInstance* instance) {
    if (_is_spawning) {
        if (_cooldown.isTriggered(0.5)) {
            Vector2D position =
                            getPosition() +
                            static_cast<float>(getHeight()) *
                            Vector2D::down() +
                            static_cast<float>(getWidth() / 5 * _spawn_pos) *
                            Vector2D::right();
            instance->getSpawner()->newEntity(
                EntityType::ShipSwarmling,
                position
            );
            ++_spawn_pos;
            if (_spawn_pos > 5) {
                _is_spawning = false;
    			// the y position is used as a bool, slighty changed to know which appearance to have
                GameEntity::setPosition(Vector2D(getX(), static_cast<float>(0.01)));
            }
        }
    } else {
        if (_cooldown.isTriggered(3.) && getY() >= 0) {
            _is_spawning = true;
            _spawn_pos   = -1;
            GameEntity::setPosition(Vector2D(getX(), 0));
        }
    }
}


#ifdef SERVER
void Game::AsteroidBase::onUpdate(Game::GameInstance* instance) {
    spawn(instance);
    if(getY() < 0)  // not fully on screen
        setPosition(getPosition()+getSpeed()/25);
}
#else

void Game::AsteroidBase::onUpdate(Game::GameInstance*) {
    GameEntity::onUpdate();
}

#endif