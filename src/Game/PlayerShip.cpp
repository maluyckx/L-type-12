#include "PlayerShip.hpp"
#include "Server/GameInstance.hpp"
#include "../Utils/DebugLogger.hpp"
#include "Client/Sound.hpp"
#include "Client/SFML/ParticleHandler.hpp"
#include "Command.hpp"
#include "Server/GameSession.hpp"
#include "Bonus.hpp"

Game::PlayerShip::PlayerShip(Game::EntityType type, std::size_t id,
                             Game::Vector2D position) :
    Ship(type,
         id,
         EntityDimensions::ShipPlayer1_WIDTH,
         EntityDimensions::ShipPlayer1_HEIGHT,
         position, STYLE_PLAYERSHIP,
         type == EntityType::ShipPlayer1 ?
         CollisionMask::RESISTANCE:
         CollisionMask::BOUNTY_HUNTER,
         1,
         ProjectileType::Normal,
         false, "sprites/spaceship/Player/spaceship") {

}

void Game::PlayerShip::draw(sf::RenderWindow* window) {
    if(window) {
        GameEntity::pencil(*this, window);
    } else {
        GameEntity::pencil(*this,
            TERMINAL_COLORS::CYAN);
    }
}

void Game::PlayerShip::shoot(GameInstance* instance) {
    if (_shoot_cooldown.isTriggered(Projectile::getAttackSpeed(getProjectile()) / 4)) {
        Vector2D position = getPosition() +
                            Vector2D(static_cast<float>(getWidth() / 2), Projectile::getDimensions(getProjectile()).getY()*-1);
        instance->getSpawner()->newEntity(
            getProjectile(),
            position,
            false,
            this
        );
    }
}

bool checkIfAlwaysInScreen(Game::Vector2D position){
    if (position.getX() < 0 ||
        position.getX() + Game::EntityDimensions::ShipPlayer1_WIDTH >= WIDTH_WINDOW ||
        position.getY() < 0 ||
        position.getY() + Game::EntityDimensions::ShipPlayer1_HEIGHT >= HEIGHT_WINDOW - Game::EntityDimensions::HUD_HEIGHT) {
            return false;
        }
        return true;
}

#ifdef SERVER
void Game::PlayerShip::onUpdate(GameInstance* instance) {
    if (getType() == EntityType::ShipPlayer1) {
        if (instance->getInput()->isKeyDown(GAME_KEYS::UP_P1)) {
            DEBUGMSG("Player1 UP was pressed");
            Vector2D new_pos = getPosition() + Vector2D::up() /2 * NORMALISING;
            if(checkIfAlwaysInScreen(new_pos)) setPosition(new_pos);
        }
        if (instance->getInput()->isKeyDown(GAME_KEYS::DOWN_P1)) {
            DEBUGMSG("Player1 DOWN was pressed");
            Vector2D new_pos = getPosition() + Vector2D::down() /2 * NORMALISING;
            if(checkIfAlwaysInScreen(new_pos)) setPosition(new_pos);
        }
        if (instance->getInput()->isKeyDown(GAME_KEYS::RIGHT_P1)) {
            DEBUGMSG("Player1 RIGHT was pressed");
            Vector2D new_pos = getPosition() + Vector2D::right() * NORMALISING;
            if(checkIfAlwaysInScreen(new_pos)) setPosition(new_pos);
        }
        if (instance->getInput()->isKeyDown(GAME_KEYS::LEFT_P1)) {
            DEBUGMSG("Player1 LEFT was pressed");
            Vector2D new_pos = getPosition() + Vector2D::left() * NORMALISING;
            if(checkIfAlwaysInScreen(new_pos)) setPosition(new_pos);
        }
        if (instance->getInput()->isKeyDown(GAME_KEYS::SHOOT_P1)) {
            DEBUGMSG("Player1 SHOOT was pressed");
            shoot(instance);
        }
    } else {
        // player2
        if (instance->getInput()->isKeyDown(GAME_KEYS::UP_P2)) {
            DEBUGMSG("Player2o UP was pressed");
            Vector2D new_pos = getPosition() + Vector2D::up() /2 * NORMALISING;
            if(checkIfAlwaysInScreen(new_pos)) setPosition(new_pos);
        }
        if (instance->getInput()->isKeyDown(GAME_KEYS::DOWN_P2)) {
            DEBUGMSG("Player2 DOWN was pressed");
            Vector2D new_pos = getPosition() + Vector2D::down() /2 * NORMALISING;
            if(checkIfAlwaysInScreen(new_pos)) setPosition(new_pos);
        }
        if (instance->getInput()->isKeyDown(GAME_KEYS::RIGHT_P2)) {
            DEBUGMSG("Player2 RIGHT was pressed");
            Vector2D new_pos = getPosition() + Vector2D::right() * NORMALISING;
            if(checkIfAlwaysInScreen(new_pos)) setPosition(new_pos);
        }
        if (instance->getInput()->isKeyDown(GAME_KEYS::LEFT_P2)) {
            DEBUGMSG("Player2 LEFT was pressed");
            Vector2D new_pos = getPosition() + Vector2D::left() * NORMALISING;
            if(checkIfAlwaysInScreen(new_pos)) setPosition(new_pos);
        }
        if (instance->getInput()->isKeyDown(GAME_KEYS::SHOOT_P2)) {
            DEBUGMSG("Player2 SHOOT was pressed");
            shoot(instance);
        }
    }
}
#else
void Game::PlayerShip::onUpdate(GameInstance*) {
    GameEntity::onUpdate();
}
#endif

void Game::PlayerShip::moveToCenter() {
    if (getType() == EntityType::ShipPlayer1) setPosition(INITIAL_POSITION_P1);
    else if (getType() == EntityType::ShipPlayer2)
        setPosition(INITIAL_POSITION_P2);
}

void Game::PlayerShip::onDeath(Game::GameInstance* instance) {
    lostLife(1);
    nuke(instance);
    instance->getGameState()->getHUD()->onUpdate(instance);
    instance->getConnection()->getSendingCommandQueue()->push(
        CommandFactory::playSound(Game::Client::SoundEffect::HitEnemySound)
    );
    if(instance->getConnection()->getOwner<Game::Server::GameSession>()->isDisplayGraphical()) {
		instance->getConnection()->getSendingCommandQueue()->push(
		    CommandFactory::makeParticle(SFML::Particle::MassiveExplosion,
		                                 getX(),
		                                 getY(),
		                                 10,
		                                 10,
		                                 false));
	}
    moveToCenter();
}

void Game::PlayerShip::onCollision(Game::GameEntity* other,
                                   Game::GameInstance* instance) {
    if (!isDestroyed()) {
        if (other) {
            bool is_friendly_fire_activated =
                     instance->getGameState()->getGameSettings().getFriendlyFire()
                     == GameSettings::FriendlyFire::Yes;
            switch (GameEntity::getGenericType(other->getType())) {
                case GenericType::PlayerShip: {
                    if (is_friendly_fire_activated) {
                        onDeath(instance);
                    }
                    break;
                }
                case GenericType::EnemyShip:
                case GenericType::Boss: {
                    onDeath(instance);
                    break;
                }
                case GenericType::Bonus: {
                    if(other->getType()==EntityType::BonusHeal) {
                        int max_life = static_cast<int>(instance->getGameState()
                                                                ->getGameSettings().getHealthPoints()
                        );
                        int new_life = getLife() + 1;
                        if (new_life < max_life)
                            setLife(new_life);
                    } else
                    	setProjectile(dynamic_cast<Bonus*>(other)->getProjectileBonus());
                    break;
                }
                case GenericType::Obstacle:
                case GenericType::HUD: {
                    onDeath(instance);
                    break;
                }
                case GenericType::Projectile: {
                    auto projectile = dynamic_cast<Projectile*>(other);
                    if (projectile->isHostile() || (is_friendly_fire_activated && projectile->getShooter() != this)) {
                        onDeath(instance);
                    }
                    break;
                }
                default:
                    break;
            }
        } else {
            onDeath(instance);
        }
        if (!getLife()) {
            destroy();
        }
    }
}

void Game::PlayerShip::nuke(GameInstance* instance) {
    for (auto entity: instance->getGameState()->getAllEntities()) {
        auto is_enemy_ship         = GameEntity::getGenericType(entity->getType()) == GenericType::EnemyShip;
        auto is_hostile_projectile = (
            GameEntity::getGenericType(entity->getType()) == GenericType::Projectile &&
            dynamic_cast<Projectile*>(entity)->isHostile()
        );
        auto is_asteroid           = entity->getType() == EntityType::Asteroid;
        if (is_enemy_ship || is_hostile_projectile || is_asteroid) {
            if(instance->getConnection()->getOwner<Game::Server::GameSession>()->isDisplayGraphical() && !is_hostile_projectile) {
                float radius = static_cast<float>(entity->getWidth() * entity->getHeight() * 2);
                if(entity->getType()==EntityType::ShipBattleship)
                	radius*=0.25f;
                instance->getConnection()->getSendingCommandQueue()->push(
                    CommandFactory::makeParticle(SFML::Particle::MassiveExplosion,
                                                entity->getX(),
                                                entity->getY(),
                                                radius, radius)
                );
            }
            entity->destroy();
        }
    }
}

void Game::PlayerShip::increaseScore(Game::GameInstance* instance,
                                     Game::PlayerShip* shooter,
                                     double score_multiplier) {
    if (shooter) {
        int curr_score = shooter->getScore();
        int curr_hp    = shooter->getLife();
        int max_hp     = instance->getGameState()
                                 ->getGameSettings().getHealthPoints();
        int difficulty = instance->getGameState()
                                     ->getGameSettings().getDifficulty();
        // formula:
        // curr_score + (curr_hp/max_hp) * (score_multi/max_hp) * difficulty =
        // = curr_score + (score_multi * curr_hp * difficulty / max_hp^2)
        // (curr_hp/max_hp) : higher the less hp we lost
        // (score_multi/max_hp) : higher the less max_hp we have
        // (difficulty) : higher the more difficult the game is
        shooter->setScore(
            static_cast<int>(
                curr_score +
                (score_multiplier * curr_hp * difficulty) /
                (max_hp * max_hp)
            )
        );
    }
}
