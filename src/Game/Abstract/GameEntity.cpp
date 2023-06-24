#include "GameEntity.hpp"
#include "../PlayerShip.hpp"
#include "../HUD.hpp"
#include "Boss.hpp"
#include "../EnemyType/Obstacle.hpp"
#include "../EnemyType/Swarmling.hpp"
#include "../EnemyType/BattleShip.hpp"
#include "../EnemyType/Escorter.hpp"
#include "../EnemyType/Cruiser.hpp"
#include "../EnemyType/LaserGun.hpp"
#include "../EnemyType/BossPoulpe.hpp"
#include "../EnemyType/AsteroidBase.hpp"
#include "../EnemyType/Enterprise.hpp"
#include "../EnemyType/Dragon.hpp"
#include "../Server/GameInstance.hpp"
#include "../Bonus.hpp"
#include <ncurses.h>
#include <iostream>
#include <utility>
#include "../../Utils/DebugLogger.hpp"

Game::GameEntity::GameEntity(EntityType type, std::size_t id,
                   int width, int height,
                   Vector2D pos, std::string style, CollisionMask mask, std::string path) :
    _type(type), _id(id), _width(width), _height(height), 
    _mask(mask), _pos(pos), _pos_serv(pos), _style(style) { 
    if(type!=EntityType::BossDragon)  // entity with animation, multiple images
        setSprite(path, (type!=EntityType::ShipPlayer1 && type!=EntityType::ShipPlayer2 && type!=EntityType::BossPoulpe && type!=EntityType::BossAsteroidBase && type!=EntityType::BossEnterprise && type!=EntityType::HUD));
    else
    	_sprite = new SFML::SpriteSimple(getX()*GRAPHIC_X_MULTIPLIER, getY()*GRAPHIC_Y_MULTIPLIER, static_cast<float>(getWidth())*GRAPHIC_X_MULTIPLIER, static_cast<float>(getHeight())*GRAPHIC_Y_MULTIPLIER, path, 2);
    }

Game::GameEntity::GameEntity(ProjectileType type, std::size_t id, Game::Vector2D pos, std::string style, bool is_hostile, Vector2D speed, Vector2D dimensions, std::string path) :
    _type(Projectile::toEntityType(type)), _id(id), _width(static_cast<int>(dimensions.getX())), _height(static_cast<int>(dimensions.getY())), 
    _mask(CollisionMask::PEW_PEW), _pos(pos), _pos_serv(pos), _style(std::move(style)), _speed(speed) {
    if(!is_hostile) {
        _speed *= -1;
    }
    if(type==ProjectileType::Fire) {  //fire explosion animated with multiple images
    	_sprite = new SFML::SpriteSimple(getX()*GRAPHIC_X_MULTIPLIER, getY()*GRAPHIC_Y_MULTIPLIER, static_cast<float>(getWidth()*5)*GRAPHIC_X_MULTIPLIER, static_cast<float>(getHeight()*2)*GRAPHIC_Y_MULTIPLIER, path, 72);  // enlarges it because the image has background around it
    } else 
        setSprite(path, is_hostile);
}

Game::GameEntity::GameEntity(EntityType type, std::size_t id, Game::Vector2D pos) : // bonus
    _type(type), _id(id), _width(Game::EntityDimensions::Bonus_WIDTH), _height(Game::EntityDimensions::Bonus_HEIGHT), 
    _mask(Game::CollisionMask::OBSTACLE), _pos(pos), _pos_serv(pos) {
    if (getType() == Game::EntityType::BonusHeal) { 
        setStyle(STYLE_BONUSHEAL);
        setSprite("sprites/obstacle/bonusheal", false);
    } else { 
        setStyle(STYLE_BONUSGUN);
        setSprite("sprites/obstacle/bonusgun", false, 2);  // make it look bigger because of the glow around the image
    }
}

//static
void Game::GameEntity::pencil(GameEntity& entity, TERMINAL_COLORS color) {
    int tmp_x = static_cast<int>(entity.getX());
    int tmp_y = static_cast<int>(entity.getY());
    int color_id;
    start_color();
    // Valeurs de init pairs sont choisies arbitrairement pour ne pas entrer en conflit avec HUD
    switch (color) {
        case TERMINAL_COLORS::BLACK:
            init_pair(100, static_cast<short int>(color),
                      static_cast<short int>(TERMINAL_COLORS::BLACK));
            color_id = 100;
            break;
        case TERMINAL_COLORS::RED:
            init_pair(101, static_cast<short int>(color),
                      static_cast<short int>(TERMINAL_COLORS::BLACK));
            color_id = 101;
            break;
        case TERMINAL_COLORS::GREEN:
            init_pair(102, static_cast<short int>(color),
                      static_cast<short int>(TERMINAL_COLORS::BLACK));
            color_id = 102;
            break;
        case TERMINAL_COLORS::YELLOW:
            init_pair(103, static_cast<short int>(color),
                      static_cast<short int>(TERMINAL_COLORS::BLACK));
            color_id = 103;
            break;
        case TERMINAL_COLORS::BLUE:
            init_pair(104, static_cast<short int>(color),
                      static_cast<short int>(TERMINAL_COLORS::BLACK));
            color_id = 104;
            break;
        case TERMINAL_COLORS::MAGENTA:
            init_pair(105, static_cast<short int>(color),
                      static_cast<short int>(TERMINAL_COLORS::BLACK));
            color_id = 105;
            break;
        case TERMINAL_COLORS::CYAN:
            init_pair(106, static_cast<short int>(color),
                      static_cast<short int>(TERMINAL_COLORS::BLACK));
            color_id = 106;
            break;
        case TERMINAL_COLORS::WHITE:
            init_pair(107, static_cast<short int>(color),
                      static_cast<short int>(TERMINAL_COLORS::BLACK));
            color_id = 107;
            break;
        default:
            break;
    }
    attron(COLOR_PAIR(color_id)); attron(A_BOLD);    
    std::string style = entity.getStyle();
    for (char i : style) {
        if (i != '\n') {
            mvaddch(tmp_y, tmp_x, static_cast<chtype>(i));
            tmp_x++;;
        } else {
            tmp_x = static_cast<int>(entity.getX());
            tmp_y++;
        }
    }
    attroff(COLOR_PAIR(color_id)); attroff(A_BOLD); 
}

//static
void Game::GameEntity::pencil(Game::GameEntity& entity, sf::RenderWindow* window) {
    if(entity._sprite) {  // to prevent segfaults
    	if(entity._sprite->getRotation()==180) { // the rotation is around the center point, need to move it back to the original placement
    		entity._sprite->setPosition((entity.getX()+static_cast<float>(entity.getWidth()))*GRAPHIC_X_MULTIPLIER,
                                        (entity.getY()+static_cast<float>(entity.getHeight()))*GRAPHIC_Y_MULTIPLIER);
        } else {
            entity._sprite->setPosition(entity.getX()*GRAPHIC_X_MULTIPLIER,
                                        entity.getY()*GRAPHIC_Y_MULTIPLIER);
        } entity._sprite->draw(*window);
    }
}

void Game::GameEntity::onUpdate(GameInstance*) {
    #ifdef SERVER
    #else
    _pos = _pos_serv.blend(_pos);
    #endif
}

//static
Game::GenericType Game::GameEntity::getGenericType(EntityType type) {
    switch(type) {
        case EntityType::ProjectileSmall:
        case EntityType::ProjectileBig:
        case EntityType::ProjectileMissile:
        case EntityType::ProjectileFire:
        case EntityType::ProjectileLaser:
        case EntityType::ProjectileNormal:
            return GenericType::Projectile;
        case EntityType::ShipPlayer1:
        case EntityType::ShipPlayer2:
            return GenericType::PlayerShip;
        case EntityType::Asteroid:
            return GenericType::Obstacle;
        case EntityType::HUD:
            return GenericType::HUD;
        case EntityType::BonusWeaponMissile:
        case EntityType::BonusWeaponLaser:
        case EntityType::BonusWeaponSmall:
        case EntityType::BonusHeal:
            return GenericType::Bonus;
        case EntityType::BossAsteroidBase:
        case EntityType::BossDragon:
        case EntityType::BossPoulpe:
        case EntityType::BossEnterprise:
            return GenericType::Boss;
        default:
            return GenericType::EnemyShip;
    }
}

//static
Game::GameEntity*
Game::GameEntity::from(Game::EntityType type, unsigned long id,
                       Game::Vector2D position, GameState* game_state,
                       bool is_hostile, PlayerShip* shooter) {
    switch (getGenericType(type)) {
    	case GenericType::Projectile: {
            DEBUGMSG("Projectile created. id = " << id);
            auto projectile = new Projectile(id, position, type, is_hostile,
                               shooter);
        	return projectile;
        }
        case GenericType::PlayerShip: {
            DEBUGMSG("PlayerShip created. id = " << id);
            auto player_ship = new PlayerShip(type, id, position);
#ifdef SERVER
            player_ship->setLife(static_cast<int>(game_state->getGameSettings().getHealthPoints()));
#endif
            return player_ship;
        }
        case GenericType::EnemyShip: {
            switch(type) {
                case EntityType::ShipBattleship: {
                    DEBUGMSG("Battleship created. id = " << id);
                    auto battleship = new BattleShip(id, position, game_state->getGameSettings().getDifficulty());
                    return battleship;
                }
                case EntityType::ShipLaserGun: {
                    DEBUGMSG("LaserGun created. id = " << id);
                    auto lasergun = new LaserGun(id, position, game_state->getGameSettings().getDifficulty());
                    return lasergun;
                }
                case EntityType::ShipEscorter: {
                    DEBUGMSG("Escorter created. id = " << id);
                    auto escorter = new Escorter(id, position, game_state->getGameSettings().getDifficulty());
                    return escorter;
                }
                case EntityType::ShipCruiser: {
                    DEBUGMSG("Cruiser created. id = " << id);
                    auto cruiser = new Cruiser(id, position, game_state->getGameSettings().getDifficulty());
                    return cruiser;
                }
                case EntityType::ShipSwarmling:
                default:	 {
                    DEBUGMSG("Swarmling created. id = " << id);
                    auto swarmling = new Swarmling(id, position, game_state->getGameSettings().getDifficulty());
                    return swarmling;
                }
            }
        }
        case GenericType::Boss: {
            switch(type) {
                case EntityType::BossDragon: {
                    DEBUGMSG("Dragon created. id = " << id);
                    auto boss_dragon = new Dragon(id, game_state->getGameSettings().getDifficulty(), position);
                    return boss_dragon;
                }
                case EntityType::BossAsteroidBase: {
                    DEBUGMSG("AsteroidBase created. id = " << id);
                    auto bossasteroid = new AsteroidBase(id, game_state->getGameSettings().getDifficulty(), position);
                    return bossasteroid;
                }
                case EntityType::BossEnterprise: {
                    DEBUGMSG("Enterprise created. id = " << id);
                    auto enterprise = new Enterprise(id, game_state->getGameSettings().getDifficulty(), position);
                    return enterprise;
                }
                case EntityType::BossPoulpe:
                default: {
                    DEBUGMSG("BossPoulpe created. id = " << id);
                    auto bosspoulpe = new BossPoulpe(id, game_state->getGameSettings().getDifficulty(), position);
                    return bosspoulpe;
                }
            }
        }
        case GenericType::Bonus: {
            DEBUGMSG("Bonus created. id = " << id);
            auto bonus = new Bonus(type, id, position);
            return bonus;
        }
        case GenericType::Obstacle: {
            DEBUGMSG("Asteroid created. id = " << id);
            auto obstacle = new Obstacle(id, position);
            return obstacle;
        }
        case GenericType::HUD: {
#ifdef SERVER
            return new HUD(
                id,
                game_state->getUsernamePlayer1(),
                game_state->getUsernamePlayer2(),
                static_cast<int>(game_state->getGameSettings().getHealthPoints())
            );
#else
            return new HUD(id, "player1", "player2", 
            static_cast<int>(game_state->getGameSettings().getHealthPoints()));  
            // client will get it's HUD data from the server
#endif
        }
        default: {  // default enemy swarmling
            DEBUGMSG("Default Swarmling created. id = " << id);
            auto swarmling = new Swarmling(id, position, game_state->getGameSettings().getDifficulty());
            return swarmling;
        }
    }
}

void Game::GameEntity::setSprite(std::string path, bool enemy, float scale) {
	_sprite = new SFML::SpriteSimple(getX()*GRAPHIC_X_MULTIPLIER,
                                     getY()*GRAPHIC_Y_MULTIPLIER,
                                     static_cast<float>(getWidth())*GRAPHIC_X_MULTIPLIER*scale,
                                     static_cast<float>(getHeight())*GRAPHIC_Y_MULTIPLIER*scale, 
                                     path);
    if(enemy) 
        _sprite->rotate(180);
}

bool Game::GameEntity::isInSameMask(Game::GameEntity& other_object) noexcept {
    return _mask == other_object.getMask();
}