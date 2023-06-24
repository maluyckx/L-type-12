#include "BossPoulpe.hpp"
#include "../Vector2D.hpp"
#include "../Server/GameInstance.hpp"

Game::BossPoulpe::BossPoulpe(long unsigned int id, int diff, Vector2D position) : 
    Boss(EntityType::BossPoulpe, id, EntityDimensions::BossPoulpe_WIDTH,
         EntityDimensions::BossPoulpe_HEIGHT, position, STYLE_BOSS_POULPE, 
         15*diff, PATH_POULPE) {}

void Game::BossPoulpe::shoot(GameInstance* instance, ProjectileType type,
                             int offset) {
    if (_shoot_cooldown.isTriggered(Projectile::getAttackSpeed(type), false)) {
        if (type == ProjectileType::Big) {  // slowest attack speed of projectiles, when we reach it it stops shooting for a while
            _shoot_cooldown.reset();
            Vector2D position =
                         getPosition() +
                         Vector2D::right() *
                         static_cast<float>(15 + offset) +
                         Vector2D::down() * 7;
            instance->getSpawner()->newEntity(type,
                                              position,
                                              true);
        } else {
            if (_cooldown_spray.isTriggered()) { // shoots projectiles left then right
                _shoot_left = !_shoot_left;
                Vector2D position =
                             getPosition() +
                             Vector2D::right() *
                             static_cast<float>(15 + offset) +
                             Vector2D::down() * 7;
                instance->getSpawner()->newEntity(type,
                                                  position,
                                                  true);
            }
        }
    }
}

#ifdef SERVER
void Game::BossPoulpe::onUpdate(Game::GameInstance* instance) {
    setPosition(getPosition() + getSpeed() / 15);
    if (getY() >= static_cast<float>(getHeight()/2)) {
        if(_shoot_left)
            shoot(instance, ProjectileType::Laser, -10);
        else
            shoot(instance, ProjectileType::Laser, 10);
        shoot(instance, ProjectileType::Big, 0);
        
        if (getX() >= WIDTH_WINDOW - static_cast<float>(getWidth() * 1.25))
            setSpeed(Vector2D::left());
        else if (getX() <= static_cast<float>(getWidth() / 4))
            setSpeed(Vector2D::right());
        else if (getSpeed() != Vector2D::left())
            setSpeed(Vector2D::right());
        // bounces when nearing the edge of the screen
    } else {
        setSpeed(Vector2D::down());
    }
}
#else

void Game::BossPoulpe::onUpdate(Game::GameInstance*) {
    GameEntity::onUpdate();
}

#endif