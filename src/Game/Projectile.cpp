#include "Projectile.hpp"
#include "Server/GameInstance.hpp"
#include "Command.hpp"
#include "Client/SFML/ParticleHandler.hpp"
#include "Server/GameSession.hpp"

Game::Projectile::Projectile(std::size_t id, Vector2D pos, EntityType projectile,
                   bool is_hostile, PlayerShip* shooter) :
    Projectile(id, pos, Projectile::toProjectileType(projectile), is_hostile, shooter) {}

Game::Projectile::Projectile(std::size_t id, Vector2D pos, ProjectileType projectile,
                   bool is_hostile, PlayerShip* shooter) :
    GameEntity(projectile, id, pos, getAppearance(projectile), is_hostile,
               getMovementSpeed(projectile), getDimensions(projectile), 
               getPath(projectile, is_hostile)), 
               _dmg(getDamage(projectile)), 
               _is_hostile(is_hostile), 
               _attack_speed(getAttackSpeed(projectile)), 
               _shooter(shooter) {}

void Game::Projectile::setProjectile(Game::ProjectileType type) {
    _dmg          = getDamage(type);
    _attack_speed = getAttackSpeed(type);
    _speed        = getMovementSpeed(type);
    setStyle(getAppearance(type));
    setSprite(getPath(type));
}

int Game::Projectile::getDamage(Game::ProjectileType type) {
    switch (type) {
        case ProjectileType::Small:
            return 1;
        case ProjectileType::Big:
            return 4;
        case ProjectileType::Laser:
            return 2;
        case ProjectileType::Missile:
            return 3;
        case ProjectileType::Fire:
            return 5;
        default:  // Normal
            return 2;
    }
}

Game::Vector2D Game::Projectile::getDimensions(Game::ProjectileType type) {
    switch(type) {
        case ProjectileType::Fire:
            return Vector2D(EntityDimensions::Fire_WIDTH, EntityDimensions::Fire_HEIGHT);
        case ProjectileType::Small:
        	return Vector2D(EntityDimensions::ProjectileSmall_WIDTH, EntityDimensions::ProjectileSmall_HEIGHT);
        case ProjectileType::Laser:
        	return Vector2D(EntityDimensions::ProjectileLaser_WIDTH, EntityDimensions::ProjectileLaser_HEIGHT);
        case ProjectileType::Big:
        	return Vector2D(EntityDimensions::ProjectileBig_WIDTH, EntityDimensions::ProjectileBig_HEIGHT);
        default:
            return Vector2D(EntityDimensions::ProjectileNormal_WIDTH, EntityDimensions::ProjectileNormal_HEIGHT);
    }
}

double Game::Projectile::getAttackSpeed(Game::ProjectileType type) {
    switch (type) {
        case Game::ProjectileType::Small:
            return 1;
        case Game::ProjectileType::Big:
            return 4;
        case Game::ProjectileType::Laser:
            return 2;
        case Game::ProjectileType::Missile:
            return 3;
        case Game::ProjectileType::Fire:
            return 4;
        default:  // Normal
             return 2;
    }
}

Game::Vector2D Game::Projectile::getMovementSpeed(Game::ProjectileType type) {
    switch (type) {
        case Game::ProjectileType::Small:
            return Vector2D(0, 2);
        case Game::ProjectileType::Big:
            return Vector2D(0, 1);
        case Game::ProjectileType::Laser:
            return Vector2D(0, 2.5);
        case Game::ProjectileType::Missile:
            return Vector2D(0, 1.5);
        case Game::ProjectileType::Fire:
            return Vector2D(0, 0.5);
        default:  // Normal
            return Vector2D(0, 2);
    }
}

std::string Game::Projectile::getAppearance(Game::ProjectileType type) {
    switch (type) {
        case Game::ProjectileType::Small:
            return "o";
        case Game::ProjectileType::Big:
            return "()";
        case Game::ProjectileType::Laser:
            return "||";
        case Game::ProjectileType::Missile:
            return "!\n*";
        case Game::ProjectileType::Fire:
            return " _\n')\n(_";
        default:  // Normal
            return "|\n|"; // *
    }
}

std::string Game::Projectile::getPath(Game::ProjectileType type, bool is_hostile) {
    switch (type) {
        case Game::ProjectileType::Small:
        	if(is_hostile)
            	return "sprites/projectiles/small-enemy";
            else return "sprites/projectiles/small-ally";
        case Game::ProjectileType::Big:
        	return "sprites/projectiles/big";
        case Game::ProjectileType::Laser:
        	if(is_hostile)
            	return "sprites/projectiles/laser-enemy";
            else return "sprites/projectiles/laser-ally";
        case Game::ProjectileType::Missile:
            return "sprites/projectiles/missile";
        case Game::ProjectileType::Fire:
            return "sprites/animations/Fire/flame_";
        default:  // Normal
        	if(is_hostile)
            	return "sprites/projectiles/normal-enemy";
            else return "sprites/projectiles/normal-ally";
    }
}

void Game::Projectile::draw(sf::RenderWindow* window) {
    if(window) { 
        GameEntity::pencil(*this, window);
    }
    else {
        if (isHostile())
            GameEntity::pencil(*this, TERMINAL_COLORS::YELLOW);
        else
            GameEntity::pencil(*this, TERMINAL_COLORS::WHITE);
    }
}

void Game::Projectile::onUpdate(Game::GameInstance*) {
    #ifdef SERVER
    setPosition(getPosition() + _speed / 4);
    #else
    GameEntity::onUpdate();
    if(getProjectileType()==ProjectileType::Fire)
    	getSprite()->animate(0.04f);
    #endif
}

void Game::Projectile::onCollision(Game::GameEntity* other,
                                   Game::GameInstance* instance) {
    if (!isDestroyed()) {
        if (other) {
            // si l'entité n'est pas un mur
            switch (GameEntity::getGenericType(other->getType())) {
                case GenericType::PlayerShip: {
                    if ((isHostile() || instance->getGameState()->getGameSettings()
                                     .getFriendlyFire()) && getShooter()->getType() != other->getType()) {
                        destroy();
                    }
                    break;
                }
                case GenericType::EnemyShip:
                case GenericType::Boss: {
                    if (!isHostile()) { destroy(); }
                    break;
                }
                case GenericType::Bonus:
                    break;
                case GenericType::Obstacle:
                default: {
                    destroy();
                    break;
                }
            }
            if(isDestroyed()) {
            	if(instance->getConnection()->getOwner<Game::Server::GameSession>()->isDisplayGraphical()) {
		            instance->getConnection()->getSendingCommandQueue()->push(
		                CommandFactory::makeParticle(SFML::Particle::ProjectileExplosion,
		                                             getX(),
		                                             getY(),
		                                             3,
		                                             3,
		                                             isHostile())
		        	);
		        }
            }
        } else {
            // we hit a wall
            destroy();
        }
    }
}

Game::EntityType Game::Projectile::toEntityType(Game::ProjectileType type) {
	switch(type) {
		case ProjectileType::Small:
			return EntityType::ProjectileSmall;
		case ProjectileType::Big:
			return EntityType::ProjectileBig;
		case ProjectileType::Missile:
			return EntityType::ProjectileMissile;
		case ProjectileType::Laser:
			return EntityType::ProjectileLaser;
		case ProjectileType::Fire:
			return EntityType::ProjectileFire;
		default:
			return EntityType::ProjectileNormal;
	}
}

Game::ProjectileType Game::Projectile::toProjectileType(Game::EntityType type) {
	switch(type) {
		case EntityType::ProjectileSmall:
			return ProjectileType::Small;
		case EntityType::ProjectileBig:
			return ProjectileType::Big;
		case EntityType::ProjectileMissile:
			return ProjectileType::Missile;
		case EntityType::ProjectileLaser:
			return ProjectileType::Laser;
		case EntityType::ProjectileFire:
			return ProjectileType::Fire;
		default:
			return ProjectileType::Normal;
	}
}

Game::Projectile::~Projectile() = default;