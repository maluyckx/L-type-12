#include "GameState.hpp"
#include "HUD.hpp"
#include <iostream>
#include "../Utils/DebugLogger.hpp"


Game::GameState::GameState(char* username_player1, char* username_player2,
                           Game::GameSettings settings) :
    _all_entities(std::vector<GameEntity*>()),
    _state(GameState::State::Started),
    _next_id(1),
    _settings(settings) {
    _all_entities.reserve(1 << 8);
    strcpy(_username_player1, username_player1);
    strcpy(_username_player2, username_player2);
}

Game::GameState::~GameState() noexcept {
    for (auto entity: _all_entities) {
        delete entity;
    }
    _all_entities.clear();
}

void Game::GameState::addEntity(Game::GameEntity* entity) {
    _all_entities.push_back(entity);
}

void Game::GameState::deleteEntity(unsigned long id) {
    for (auto item = _all_entities.begin();
         item != _all_entities.end(); item++) {
        if ((*item)->getId() == id) {
            DEBUGMSG("Entity id=" << (*item)->getId() << " was destroyed");
            delete *item;
            _all_entities.erase(item);
            break;
        }
    }
}

void Game::GameState::updateGame(GameInstance* instance) {
    for (auto entity: _all_entities) {
        entity->onUpdate(instance);
    }
}

void Game::GameState::updateEntityServerPosition(unsigned long id,
                                                 Game::Vector2D position) {
    for (auto entity: _all_entities) {
        if (entity->getId() == id) {
            entity->setPositionServer(position);
            break;
        }
    }
}

void Game::GameState::updateHUD(char* username_player1, char* username_player2,
                                int player1_hp, int player1_score,
                                int player2_hp, int player2_score,
                                int boss_hp, int player_max_hp,
                                int boss_max_hp) {
    auto hud = getHUD();
    if (hud) {
        hud->updateContent(username_player1, username_player2,
                           player1_hp, player2_hp, boss_hp,
                           player1_score, player2_score, player_max_hp, boss_max_hp);
    }
}

void Game::GameState::updateHUDState() {
    auto hud = getHUD();
    if (hud) {
        if (_state == GameState::State::Paused) {
            hud->setIsPaused(true);
            hud->draw();
        } else {
            hud->setIsPaused(false);
            hud->draw();
        }
    }
}

Game::PlayerShip* Game::GameState::getPlayerShip(bool first_player) {
    PlayerShip* ship = nullptr;
    if (first_player) {
        for (auto entity: _all_entities) {
            if (entity->getType() == EntityType::ShipPlayer1) {
                ship = dynamic_cast<PlayerShip*>(entity);
                return ship;
            }
        }
    } else {
        for (auto entity: _all_entities) {
            if (entity->getType() == EntityType::ShipPlayer2) {
                ship = dynamic_cast<PlayerShip*>(entity);
                return ship;
            }
        }
    }
    return ship;
}

Game::HUD* Game::GameState::getHUD() {
    HUD* hud = nullptr;
    for (auto entity: _all_entities) {
        if (entity->getType() == EntityType::HUD) {
            hud = dynamic_cast<HUD*>(entity);
            return hud;
        }
    }
    return hud;
}

Game::Boss* Game::GameState::getBoss() {
    Boss* boss = nullptr;
    for (auto entity: _all_entities) {
        if (GameEntity::getGenericType(entity->getType()) == GenericType::Boss) {
            boss = dynamic_cast<Boss*>(entity);
            return boss;
        }
    }
    return boss;
}