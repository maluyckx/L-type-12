#include "GameInstance.hpp"
#include "../../Utils/DebugLogger.hpp"
#include "../Command.hpp"


Game::GameInstance::GameInstance(char* username_player1,
                                 char* username_player2,
                                 Game::GameSettings settings,
                                 Connection* connection) :
    _game_state(new GameState(username_player1, username_player2, settings)),
    _spawner(
        new Server::Spawner(connection, _game_state, settings.getLevelName())
    ),
    _connection(connection),
    _input(new Input()) {
}

Game::GameInstance::~GameInstance() noexcept {
    delete _game_state;
    delete _spawner;
    delete _input;
}

void Game::GameInstance::updateInput(char* inputs) {
    _input->updateInputTable(inputs);
}

void Game::GameInstance::clearInput() {
    _input->clearInputTable();
}

void Game::GameInstance::start() {
    // here we spawn player 1 and player 2
    _spawner->newEntity(EntityType::ShipPlayer1);
    if (getGameState()->getGameSettings().getMode() ==
        GameSettings::GameMode::Coop) {
        _spawner->newEntity(EntityType::ShipPlayer2);
    }
    _spawner->newEntity(EntityType::HUD);
    _started = true;
}

void Game::GameInstance::end(bool quitting, int level_remain) {
    _game_state->setState(GameState::State::Over);
    if (quitting) {
        auto* command = Game::CommandFactory::gameStatus(
            Game::Command::GameStatus::State::Quit,
            _game_state->getGameSettings(),
            strdup(""),
            strdup(""),
            0,
            0,
            level_remain
        );
        _connection->getSendingCommandQueue()->push(command);
    } else {
        int  score_1          = _game_state->getHUD()->getPlayer1Score();
        int  score_2          = _game_state->getHUD()->getPlayer2Score();
        auto username_player1 =
                 const_cast<char*>(_game_state->getUsernamePlayer1());
        auto username_player2 =
                 const_cast<char*>(_game_state->getUsernamePlayer2());
            // Le score est sauvé dans tous les cas, niveau fini ou non
            auto database = _connection->getDatabase();
            database->lock();
            if (score_1) {
                database->updateXp(username_player1, score_1);
            }
            if (score_2) {
                database->updateXp(username_player2, score_2);
            }
            database->unlock();
        auto* command = Game::CommandFactory::gameStatus(
            Game::Command::GameStatus::State::Ends,
            _game_state->getGameSettings(),
            username_player1,
            username_player2,
            score_1,
            score_2,
            level_remain
        );
        _connection->getSendingCommandQueue()->push(command);
    }
}

void Game::GameInstance::levelEnded() {
    if (!_slow_end) {
        _slow_end = new Utils::Timer(2);
    } else if (_slow_end->isTriggered()) {
        std::string level = getGameState()->getGameSettings().getLevelName();
        if (!_campaign_level) {
            _campaign_level =
                static_cast<unsigned>(level[level.size() - 5]) + 1;
        }
        if (level[level.size() - 6] == '_') {
            level[level.size() - 5] = static_cast<char>(_campaign_level);
            if (_spawner->setLevel(level)) {
                delete _slow_end;
                _slow_end = nullptr;
                _connection->getSendingCommandQueue()->push(
                    CommandFactory::playSound(
                        Client::SoundEffect::LevelChange,
                        _campaign_level % 4 + 1
                    )
                );
            } else {
                if (std::strcmp(level.c_str(), "levels/campaign_4.txt") == 0)
                    end(false, 0);
                else
                    end(false, -1);
            }
            ++_campaign_level;
        } else
            end(false, -1);  // win
    }
}

void Game::GameInstance::lost() {
    if (!_slow_end)
        _slow_end = new Utils::Timer(1);
    else if (_slow_end->isTriggered()) {
        std::string level = getGameState()->getGameSettings().getLevelName();
        if (!_campaign_level) {
            _campaign_level =
                static_cast<unsigned>(level[level.size() - 5]) + 1;
        }
        unsigned max_level = _campaign_level;
        if (level[level.size() - 6] == '_') {
            ++max_level;
            level[level.size() - 5] = static_cast<char>(max_level);
            if (!_spawner->fileExists(level))
                --max_level;  // we are at the last level
            else {
                while (_spawner->fileExists(level)) {
                    ++max_level;
                    level[level.size() - 5] = static_cast<char>(max_level);
                }
            }
        }
        end(false, static_cast<int>(max_level - _campaign_level + 1));
    }
}

void Game::GameInstance::run() {
    if (!_game_state->isPaused()) {
        getGameState()->updateGame(this);
        physicsUpdate(this);
        clearDestroyedEntities();
        sendPositionsToClient();
        updateClientHUD();
        if (!_spawner->advanceInLevel() && !getGameState()->getBoss()) {
            levelEnded();
        } else if (!arePlayersAlive()) {
            lost();
        }
    }
    handlePauseInputs();
    clearInput();
}

void Game::GameInstance::physicsUpdate(GameInstance* instance) {
    std::vector<Game::Server::CollisionedEntity> collisioned{};
    Game::Server::CollisionResolver::physicsUpdate(
        getGameState()->getAllEntities(), collisioned
    );
    for (auto collision_pair: collisioned) {
        if (collision_pair.first) {
            collision_pair.first->onCollision(collision_pair.second, instance);
        }
        if (collision_pair.second) {
            collision_pair.second->onCollision(collision_pair.first, instance);
        }
    }
}

void Game::GameInstance::handlePauseInputs() {
    if (_game_state->isPaused()) {
        if (getInput()->isKeyDown(GAME_KEYS::RESUME)) {
            DEBUGMSG("KEY RESUME was pressed");
            _game_state->setState(GameState::State::Started);
            _connection->getSendingCommandQueue()->push(
                CommandFactory::gameStatus(
                    Command::GameStatus::State::Resume,
                    _game_state->getGameSettings(),
                    const_cast<char*>(_game_state->getUsernamePlayer1()),
                    const_cast<char*>(_game_state->getUsernamePlayer2()),
                    0,
                    0,
                    false
                )
            );
        } else if (getInput()->isKeyDown(GAME_KEYS::QUIT)) {
            DEBUGMSG("KEY QUIT was pressed");
            end(true, false);
        }
    } else {
        if (getInput()->isKeyDown(GAME_KEYS::PAUSE)) {
            DEBUGMSG("KEY PAUSE was pressed");
            _game_state->setState(GameState::State::Paused);
            _connection->getSendingCommandQueue()->push(
                CommandFactory::gameStatus(
                    Command::GameStatus::State::Pause,
                    _game_state->getGameSettings(),
                    const_cast<char*>(_game_state->getUsernamePlayer1()),
                    const_cast<char*>(_game_state->getUsernamePlayer2()),
                    0,
                    0,
                    false
                )
            );
        }
    }
}

void Game::GameInstance::sendPositionsToClient() {
    for (auto entity: _game_state->getAllEntities()) {
        if (entity->getPosition() != entity->getPositionServer()) {
            _connection->getSendingCommandQueue()->push(
                CommandFactory::updateEntityPosition(
                    entity->getId(),
                    entity->getPosition()
                )
            );
        }
    }
}

void Game::GameInstance::updateClientHUD() {
    for (auto entity: _game_state->getAllEntities()) {
        if (entity->getType() == EntityType::HUD) {
            auto hud = dynamic_cast<HUD*>(entity);
            _connection->getSendingCommandQueue()->push(
                CommandFactory::updateHUD(
                    hud->getUsernamePlayer1(),
                    hud->getUsernamePlayer2(),
                    hud->getPlayer1Hp(),
                    hud->getPlayer1Score(),
                    hud->getPlayer2Hp(),
                    hud->getPlayer2Score(),
                    hud->getBossHp(),
                    hud->getPlayerHpMax(),
                    hud->getBossHpMax()
                )
            );
            break;
        }
    }
}

void Game::GameInstance::clearDestroyedEntities() {
    auto& entities = _game_state->getAllEntities();
    for (auto it = entities.begin(); it != entities.end();) {
        auto entity = *it;
        if (entity->isDestroyed()) {
            _connection->getSendingCommandQueue()->push(
                CommandFactory::destroyEntity(entity->getId())
            );
            DEBUGMSG("Entity id=" << entity->getId() << " was destroyed");
            if (GameEntity::getGenericType(entity->getType()) ==
                GenericType::PlayerShip) {
                clearProjectilesShooters(dynamic_cast<PlayerShip*>(entity));
            }
            delete entity;
            entities.erase(it);
        } else {
            it++;
        }
    }
}

void Game::GameInstance::clearProjectilesShooters(Game::PlayerShip* shooter) {
    for (auto entity: _game_state->getAllEntities()) {
        if (GameEntity::getGenericType(entity->getType()) ==
            GenericType::Projectile) {
            auto projectile = dynamic_cast<Projectile*>(entity);
            if (projectile->getShooter() == shooter) {
                projectile->deleteShooter();
            }
        }
    }
}

bool Game::GameInstance::arePlayersAlive() {
    if (!_started) {
        return true;
    }
    auto game_state = getGameState();
    auto player1    = game_state->getPlayerShip(true);
    auto player2    = game_state->getPlayerShip(false);
    return player1 || player2;
}
