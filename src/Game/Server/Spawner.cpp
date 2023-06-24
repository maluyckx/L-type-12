#include "Spawner.hpp"
#include "../Command.hpp"
#include <bits/stdc++.h>

Game::GameEntity* Game::Server::Spawner::newEntity(EntityType type,
                                                   Vector2D position,
                                                   bool is_hostile,
                                                   PlayerShip* shooter) {
    auto new_id = _game_state->getNewId();
    GameEntity* new_entity;
    switch (GameEntity::getGenericType(type)) {
        case GenericType::PlayerShip:
            if(type==EntityType::ShipPlayer1)
                position = INITIAL_POSITION_P1;
            else
                position = INITIAL_POSITION_P2;
            new_entity = GameEntity::from(type, new_id, position,_game_state);
            break;
        case GenericType::HUD:
            position   = Vector2D(0, 0);
            new_entity = GameEntity::from(type, new_id, position,
                                          _game_state);
            break;
        case GenericType::Projectile:
            new_entity = GameEntity::from(type, new_id, position,
                                          nullptr,
                                          is_hostile, shooter);
            _connection->getSendingCommandQueue()->push(
                CommandFactory::createEntity(
                    new_entity->getType(),
                    new_entity->getId(),
                    new_entity->getPosition(),
                    is_hostile
                )
            );
            if(!is_hostile)
                _connection->getSendingCommandQueue()->push(
                    CommandFactory::playSound(Game::Client::SoundEffect::ShipFire));
            _game_state->addEntity(new_entity);
            return new_entity;
        case GenericType::Boss:
            new_entity = GameEntity::from(type, new_id, position, _game_state);
            _game_state->getHUD()->setBossMaxHp(dynamic_cast<Boss*>(new_entity)->getHp());
        	_connection->getSendingCommandQueue()->push(
                CommandFactory::playSound(Client::SoundEffect::BossArrive, static_cast<unsigned int>(getBossNumber(type)))
            );
            break;
        default:
            new_entity = GameEntity::from(type, new_id, position, _game_state);
            break;
    }
    _connection->getSendingCommandQueue()->push(
        CommandFactory::createEntity(
            new_entity->getType(),
            new_entity->getId(),
            new_entity->getPosition()
        )
    );
    _game_state->addEntity(new_entity);
    return new_entity;
}

bool Game::Server::Spawner::setLevel(const std::string& path) {
    _timer.reset();
    _offset = 0;
    _time_last_spawned = 0;
    _level.clear();
    return _level.retrieveLevel(path);
}

bool Game::Server::Spawner::advanceInLevel() {
    if (_timer.isTriggered()) {
        if (_level.getSize() > _offset) {
            while (_level.getSize() > _offset &&
                   std::get<0>(_level[_offset]) == static_cast<int>(_time_last_spawned)) {
                float y = 0;
                if(std::get<1>(_level[_offset]) == EntityType::BossPoulpe)
                    y = -EntityDimensions::BossPoulpe_HEIGHT;
                else if(std::get<1>(_level[_offset]) == EntityType::BossDragon)
                    y = -EntityDimensions::BossDragon_HEIGHT;
                else if(std::get<1>(_level[_offset]) == EntityType::BossAsteroidBase)
                	y = -EntityDimensions::BossAsteroidBase_HEIGHT;
                else if(std::get<1>(_level[_offset]) == EntityType::BossAsteroidBase)
                	y = EntityDimensions::BossEnterprise_HEIGHT;
                newEntity(std::get<1>(_level[_offset]),
                          Vector2D(static_cast<float>(std::get<2>(_level[_offset])), y));
                ++_offset;
            }
        } else {
            return false;
        }
        ++_time_last_spawned;
    }
    return true;
}

int Game::Server::Spawner::getBossNumber(const EntityType boss_type) {
    switch(boss_type) {
        case EntityType::BossDragon:
            return 1;
        case EntityType::BossEnterprise:
            return 2;
        case EntityType::BossPoulpe:
            return 3;
        case EntityType::BossAsteroidBase:
        default:
            return 4;
    }
}