#include "Connection.hpp"
#include "Command.hpp"
#include "Client/GameClient.hpp"
#include "Client/Window.hpp"
#include "Client/SFML/GraphicalWindow.hpp"
#include <memory.h>
#include <iostream>
#include <string>
#include "../Utils/DebugLogger.hpp"
#include "../Utils/Constants.hpp"
#include "EnemyType/Dragon.hpp"


void Game::CommandFactory::serialize(Game::Command* command, char* buffer) {
    switch (command->type) {
        default: {
            memcpy(buffer, command, sizeof(Command));
            break;
        }
    }
    delete command;
}

Game::Command* Game::CommandFactory::deserialize(char* buffer) {
    auto command = new Command();
    memcpy(command, buffer, sizeof(Command));
    return command;
}

Game::Command* Game::CommandFactory::ping() {
    auto* command = new Command();
    command->type      = Command::Type::Ping;
    command->data.ping = Command::Ping();
    command->data.ping.chrono = Utils::StopWatch();
    command->data.ping.chrono.start();
    return command;
}

Game::Command*
Game::CommandFactory::createAccount(char* username, char* password,
                                    char* secret_question,
                                    char* secret_answer) {
    auto command = new Command();
    command->type                      = Command::Type::CreateAccount;
    command->data.create_account.state = Command::CreateAccount::State::Request;
    strcpy(command->data.create_account.username, username);
    strcpy(command->data.create_account.password, password);
    strcpy(command->data.create_account.secret_question, secret_question);
    strcpy(command->data.create_account.secret_answer, secret_answer);
    return command;
}

Game::Command* Game::CommandFactory::authLogin(char* username, char* password) {
    auto command                   = new Command();
    command->type                  = Command::Type::AuthLogin;
    command->data.auth_login.state = Command::AuthLogin::State::Undefined;
    strcpy(command->data.auth_login.username, username);
    strcpy(command->data.auth_login.password, password);
    return command;
}

Game::Command* Game::CommandFactory::authLogout(char* username) {
    auto command  = new Command();
    command->type = Command::Type::AuthLogout;
    strcpy(command->data.auth_logout.username, username);
    return command;
}

Game::Command*
Game::CommandFactory::authRestore(Game::Command::AuthRestore::State state,
                                  char* username, char* new_password,
                                  char* secret_question, char* secret_answer) {
    auto command                     = new Command();
    command->type                    = Command::Type::AuthRestore;
    command->data.auth_restore.state = state;
    strcpy(command->data.auth_restore.username, username);
    strcpy(command->data.auth_restore.new_password, new_password);
    strcpy(command->data.auth_restore.secret_question, secret_question);
    strcpy(command->data.auth_restore.secret_answer, secret_answer);
    return command;
}

Game::Command*
Game::CommandFactory::sendError(char* error) {
    auto command  = new Command();
    command->type = Command::Type::SendError;
    strcpy(command->data.send_error.error, error);
    return command;
}

Game::Command*
Game::CommandFactory::gameStatus(Game::Command::GameStatus::State state,
                                 Game::GameSettings settings,
                                 char* username_player1,
                                 char* username_player2,
                                 int score_player1,
                                 int score_player2,
                                 int level_remain) {
    auto command                       = new Command();
    command->type                      = Command::Type::GameStatus;
    command->data.game_status.state    = state;
    command->data.game_status.settings = settings;
    strcpy(command->data.game_status.username_player1, username_player1);
    strcpy(command->data.game_status.username_player2, username_player2);
    command->data.game_status.score_player1 = score_player1;
    command->data.game_status.score_player2 = score_player2;
    command->data.game_status.level_remain      = level_remain;
    return command;
}

Game::Command* Game::CommandFactory::userFriendList(char* username) {
    auto command  = new Command();
    command->type = Command::Type::UserFriendList;
    command->data.user_friendlist.state =
        Command::UserFriendList::State::Request;
    strcpy(command->data.user_friendlist.username, username);
    return command;
}

Game::Command* Game::CommandFactory::updateEntityPosition(unsigned long id,
                                                          Game::Vector2D position) {
    auto command = new Command();
    command->type                                 = Command::Type::UpdateEntityPosition;
    command->data.update_entity_position.id       = id;
    command->data.update_entity_position.position = position;
    return command;
}

Game::Command* Game::CommandFactory::userData(char* username) {
    auto command                  = new Command();
    command->type                 = Command::Type::UserData;
    command->data.user_data.state = Command::UserData::State::Request;
    strcpy(command->data.user_data.username, username);
    return command;
}

Game::Command*
Game::CommandFactory::friendRequest(Game::Command::FriendRequest::State state,
                                    char* from_username, char* to_username) {
    auto command                       = new Command();
    command->type                      = Command::Type::FriendRequest;
    command->data.friend_request.state = state;
    strcpy(command->data.friend_request.from_username, from_username);
    strcpy(command->data.friend_request.to_username, to_username);
    return command;
}

Game::Command*
Game::CommandFactory::friendProfileInfo(char* from_username,
                                        char* to_username) {
    auto command  = new Command();
    command->type = Command::Type::FriendProfileInfo;
    strcpy(command->data.friend_profile.from_username, from_username);
    strcpy(command->data.friend_profile.to_username, to_username);
    return command;
}

Game::Command* Game::CommandFactory::leaderboard() {
    auto command                    = new Command();
    command->type                   = Command::Type::Leaderboard;
    command->data.leaderboard.state = Command::Leaderboard::State::Request;
    return command;
}

Game::Command*
Game::CommandFactory::createEntity(Game::EntityType type, unsigned long id,
                                   Game::Vector2D position,
                                   bool is_hostile) {
    auto command  = new Command();
    command->type = Command::Type::CreateEntity;
    command->data.create_entity.type     = type;
    command->data.create_entity.id       = id;
    command->data.create_entity.position = position;
    if(GameEntity::getGenericType(type) == GenericType::Projectile)
        command->data.create_entity.is_hostile = is_hostile;
    return command;
}

Game::Command* Game::CommandFactory::destroyEntity(unsigned long id) {
    auto command                    = new Command();
    command->type                   = Command::Type::DestroyEntity;
    command->data.destroy_entity.id = id;
    return command;
}

Game::Command* Game::CommandFactory::input(char* inputs) {
    auto command  = new Command();
    command->type = Command::Type::Input;
    strcpy(command->data.input.chars, inputs);
    return command;
}

Game::Command*
Game::CommandFactory::updateHUD(const char* username_player1,
                                const char* username_player2,
                                int player1_hp, int player1_score,
                                int player2_hp, int player2_score,
                                int boss_hp, int player_max_hp,
                                int boss_max_hp) {
    auto command  = new Command();
    command->type = Command::Type::UpdateHUD;
    strcpy(command->data.update_hud.username_player1, username_player1);
    strcpy(command->data.update_hud.username_player2, username_player2);
    command->data.update_hud.player1_hp    = player1_hp;
    command->data.update_hud.player2_hp    = player2_hp;
    command->data.update_hud.player1_score = player1_score;
    command->data.update_hud.player2_score = player2_score;
    command->data.update_hud.boss_hp       = boss_hp;
    command->data.update_hud.player_max_hp = player_max_hp;
    command->data.update_hud.boss_max_hp   = boss_max_hp;
    return command;
}

Game::Command*
Game::CommandFactory::createLevel(const char* username_creator,
                                const char* name_level,
                                const char* level,
                                bool check_name) {
    auto command  = new Command();
    command->type = Command::Type::CreateLevel;
    strcpy(command->data.create_level.username_creator, username_creator);
    strcpy(command->data.create_level.name_level, name_level);
    strcpy(command->data.create_level.level, level);
    if (check_name) command->data.create_level.state = Command::CreateLevel::State::Check;
    return command;
}

Game::Command*
Game::CommandFactory::userLevelList(const char* username_creator){
    auto command  = new Command();
    command->type = Command::Type::UserLevelList;
    strcpy(command->data.user_level_list.username_creator, username_creator);
    return command;
}

Game::Command*
Game::CommandFactory::likeLevel(
            const char* username_liker,
            const char* name_level) {
    auto command  = new Command();
    command->type = Command::Type::LikeLevel;
    strcpy(command->data.like_level.username_liker, username_liker);
    strcpy(command->data.like_level.level_name, name_level);
    return command;
}

Game::Command* Game::CommandFactory::isliked(const char* username_liker,
                                             const char* name_level){
    auto command  = new Command();
    command->type = Command::Type::IsLiked;
    strcpy(command->data.is_liked.username_liker, username_liker);
    strcpy(command->data.is_liked.level_name, name_level);
    return command;
}

Game::Command*
Game::CommandFactory::playSound(Game::Client::SoundEffect sound, unsigned level_number) {
    auto command = new Command();
    command->type = Command::Type::PlaySound;
    command->data.play_sound.sound        = sound;
    command->data.play_sound.level_number = level_number;
    return command;
}

Game::Command* Game::CommandFactory::makeParticle(SFML::Particle type, float x, float y, float w, float h, bool is_hostile) {
    auto command = new Command();
    command->type = Command::Type::Particle;
    command->data.particle.type = type;
    command->data.particle.x = x;
    command->data.particle.y = y;
    command->data.particle.w = w;
    command->data.particle.h = h;
    command->data.particle.reverse = is_hostile;
    return command;
}

void Game::CommandExecutor::execute(Game::Connection* connection,
                                    Game::Command* command) {
    switch (command->type) {
#ifdef SERVER
#ifdef DEBUG
        case Command::Type::Ping: {
            connection->getSendingCommandQueue()->push(command);
            break;
        }
#endif // DEBUG
        case Command::Type::Input: {
            auto session = connection->getOwner<Server::GameSession>();
            auto game    = session->getGameInstance();
            if (game) { game->updateInput(command->data.input.chars); }
            delete command;
            break;
        }
        case Command::Type::CreateAccount: {
            auto database = connection->getDatabase();
            database->lock();
            bool result = database->signIn(
                command->data.create_account.username,
                command->data.create_account.password,
                command->data.create_account.secret_question,
                command->data.create_account.secret_answer
            );
            database->unlock();
            if (result) {
                command->data.create_account.state =
                    Command::CreateAccount::State::Success;
            } else {
                command->data.create_account.state =
                    Command::CreateAccount::State::Failed;
            }
            connection->getSendingCommandQueue()->push(command);
            break;
        }

        case Command::Type::AuthLogin: {
            auto session  = connection->getOwner<Server::GameSession>();
            auto database = connection->getDatabase();
            database->lock();
            int  user_id = database->getId(command->data.auth_login.username);
            bool result  =
                     database->logIn(
                         command->data.auth_login.username,
                         command->data.auth_login.password
                     ) &&
                     !session->isUserLoggedIn(user_id);
            database->unlock();
            if (result) {
                command->data.auth_login.state =
                    Command::AuthLogin::State::Accepted;
                session->logInUser(user_id);
            } else {
                command->data.auth_login.state =
                    Command::AuthLogin::State::Denied;
            }
            connection->getSendingCommandQueue()->push(command);
            break;
        }

        case Command::Type::AuthLogout: {
            auto session = connection->getOwner<Server::GameSession>();
            auto database = connection->getDatabase();
            database->lock();
            session->logOutUser(
                database->getId(command->data.auth_logout.username)
                );
            database->unlock();
            delete command;
            break;
        }

        case Command::Type::AuthRestore: {
            auto database = connection->getDatabase();
            switch (command->data.auth_restore.state) {
                case Command::AuthRestore::State::RequestSecretQuestion: {
                    database->lock();
                    auto secret_question             =
                             database->getSecretQuestion(
                                 command->data.auth_restore.username
                             );
                    database->unlock();
                    // secret question is empty if it was not found in the
                    // database, the client need to handle it
                    strcpy(command->data.auth_restore.secret_question,
                           secret_question.c_str());
                    command->data.auth_restore.state =
                        Command::AuthRestore::State::ResponseSecretQuestion;
                    connection->getSendingCommandQueue()->push(command);
                    break;
                }
                case Command::AuthRestore::State::CheckSecretAnswer: {
                    database->lock();
                    auto secret_answer = database->getSecretAnswer(
                        command->data.auth_restore.username
                    );
                    database->unlock();
                    if (strcmp(secret_answer.c_str(),
                               command->data.auth_restore.secret_answer) == 0) {
                        // secret answer is correct
                        command->data.auth_restore.state =
                            Command::AuthRestore::State::SecretAnswerAccepted;
                    } else {
                        // secret answer is incorrect
                        command->data.auth_restore.state =
                            Command::AuthRestore::State::SecretAnswerDenied;
                    }
                    connection->getSendingCommandQueue()->push(command);
                    break;
                }
                case Command::AuthRestore::State::NewPassword: {
                    database->lock();
                    database->changePasswd(
                        command->data.auth_restore.username,
                        command->data.auth_restore.new_password
                    );
                    database->unlock();
                    delete command;
                    break;
                }
                default: {
                    // we shouldn't be here
                    break;
                }
            }
            break;
        }
        case Command::Type::SendError: {
            connection->getSendingCommandQueue()->push(command);
            break;
        }
        case Command::Type::UserData: {
            // missing user looks like User res(-1, "MissingNo.");
            auto database = connection->getDatabase();
            database->lock();
            auto user = database->getUser(
                command->data.user_data.username);
            database->unlock();
            bool user_was_not_found = user.getId() == -1;
            if (user_was_not_found) {
                command->data.user_data.state =
                    Command::UserData::State::NotFound;
            } else {
                command->data.user_data.state =
                    Command::UserData::State::Completed;
                database->lock();
                auto pending_requests         = database->getPendingRequest(
                    command->data.user_data.username
                );
                auto friend_list              = database->getFriendList(
                    command->data.user_data.username
                );
                database->unlock();
                command->data.user_data.id = user.getId();
                command->data.user_data.xp = user.getXp();
                strcpy(
                    command->data.user_data.password,
                    user.getPassword().c_str()
                );
                strcpy(
                    command->data.user_data.secret_question,
                    user.getSecretQuestion().c_str()
                );
                strcpy(
                    command->data.user_data.secret_answer,
                    user.getSecretAnswer().c_str()
                );
                for (unsigned long i = 0; i < friend_list.size(); ++i) {
                    strcpy(
                        command->data.user_data.friend_list
                        + i * USERNAME_MAX_SIZE,
                        friend_list[i].c_str()
                    );
                }
                for (unsigned long i = 0;
                     i < pending_requests.size(); ++i) {
                    strcpy(
                        command->data.user_data.pending_friend_requests
                        + i * USERNAME_MAX_SIZE,
                        pending_requests[i].c_str()
                    );
                }

            }
            connection->getSendingCommandQueue()->push(command);
            break;
        }
        case Command::Type::UserFriendList: {
            if (command->data.user_friendlist.state ==
                Command::UserFriendList::State::Request) {
                auto database = connection->getDatabase();
                database->lock();
                auto friend_list = database->getFriendList(
                    command->data.user_friendlist.username);
                database->unlock();
                command->data.user_friendlist.friends_count =
                    static_cast<unsigned>(friend_list.size());
                for (unsigned i = 0;
                     i < command->data.user_friendlist.friends_count;
                     ++i) {
                    strcpy(command->data.user_friendlist.friend_list +
                           i * USERNAME_MAX_SIZE,
                           friend_list[i].c_str());
                }
                // TODO: what to do if the user was not found?
                command->data.user_friendlist.state =
                    Command::UserFriendList::State::Completed;
                connection->getSendingCommandQueue()->push(command);
            }
            break;
        }
        case Command::Type::FriendProfileInfo: {
            auto database = connection->getDatabase();
            database->lock();
            auto user_friend_list              = database->getFriendList(
                command->data.friend_profile.from_username);
            auto friend_friend_list            = database->getPendingRequest(
                command->data.friend_profile.to_username);
            auto friend_info                   = database->getUser(
                command->data.friend_profile.from_username);
            database->unlock();
            // find state
            command->data.friend_profile.state =
                Command::FriendProfileInfo::State::NOT_FRIEND;
            command->data.friend_profile.score = friend_info.getXp();
            for (size_t i = 0; i < user_friend_list.size(); i++) {
                if (user_friend_list[i].compare(
                    command->data.friend_profile.to_username) == 0)
                    command->data.friend_profile.state =
                        Command::FriendProfileInfo::State::FRIEND;
            }
            if (command->data.friend_profile.state ==
                Command::FriendProfileInfo::State::NOT_FRIEND) {
                for (size_t i = 0; i < friend_friend_list.size(); i++) {
                    if (friend_friend_list[i].compare(
                        command->data.friend_profile.from_username) == 0)
                        command->data.friend_profile.state =
                            Command::FriendProfileInfo::State::PENDING;
                }
            }
            connection->getSendingCommandQueue()->push(command);
            break;
        }
        case Command::Type::GameStatus: {
            if (command->data.game_status.state ==
                Command::GameStatus::State::Starts) {
                auto session = connection->getOwner<Server::GameSession>();
                session->startGame(
                    command->data.game_status.username_player1,
                    command->data.game_status.username_player2,
                    command->data.game_status.settings
                );
                // notify the client that the game has started
                connection->getSendingCommandQueue()->push(command);
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::ClientInitialized) {
                auto session = connection->getOwner<Server::GameSession>();
                session->getGameInstance()->start();
                delete command;
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::Pause) {
                connection->getSendingCommandQueue()->push(command);
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::Resume) {
                connection->getSendingCommandQueue()->push(command);
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::Quit) {
                connection->getSendingCommandQueue()->push(command);
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::Ends) {
                auto database = connection->getDatabase();
                database->lock();
                database->updateXp(
                    command->data.game_status.username_player1,
                    command->data.game_status.score_player1
                );
                // If two players
                if (command->data.game_status.username_player2[0] != '\0') {
                    database->updateXp(
                        command->data.game_status.username_player2,
                        command->data.game_status.score_player2
                    );
                }
                database->unlock();
                connection->getSendingCommandQueue()->push(command);
            }
            break;
        }
        case Command::Type::FriendRequest: {
            auto database = connection->getDatabase();
            switch (command->data.friend_request.state) {
                case Command::FriendRequest::State::Pending: {
                    // the client has sent a new friend request
                    database->lock();
                    auto pending_friend_requests = database->getPendingRequest(
                        command->data.friend_request.from_username
                    );
                    database->unlock();
                    // ignore if we have too many friend requests (avoids
                    // segfault)
                    if (pending_friend_requests.size() <
                        MAX_FRIENDREQUEST_COUNT) {
                        database->lock();
                        database->addFriendRequest(
                            command->data.friend_request.from_username,
                            command->data.friend_request.to_username
                        );
                        database->unlock();
                    }
                    delete command;
                    break;
                }
                case Command::FriendRequest::State::Accepted: {
                    // the client has accepted a friend request
                    database->lock();
                    database->acceptFriendRequest(
                        command->data.friend_request.from_username,
                        command->data.friend_request.to_username
                    );
                    database->unlock();
                    delete command;
                    break;
                }
                case Command::FriendRequest::State::Denied: {
                    // the client denied a friend request
                    database->lock();
                    database->refuseFriendRequest(
                        command->data.friend_request.from_username,
                        command->data.friend_request.to_username
                    );
                    database->unlock();
                    delete command;
                    break;
                }
                case Command::FriendRequest::State::Delete: {
                    // the client denied a friend request
                    database->lock();
                    database->deleteFriend(
                        command->data.friend_request.from_username,
                        command->data.friend_request.to_username
                    );
                    database->unlock();
                    delete command;
                    break;
                }
            }
            break;
        }
        case Command::Type::Leaderboard: {
            if (command->data.leaderboard.state ==
                Command::Leaderboard::State::Request) {
                auto database = connection->getDatabase();
                database->lock();
                auto leaderboard = database->getLeaderboard();
                database->unlock();
                command->data.leaderboard.top_player_count =
                    leaderboard.size() < LEADERBOARD_MAX_COUNT ?
                    static_cast<unsigned>(leaderboard.size()) :
                    LEADERBOARD_MAX_COUNT;
                for (unsigned long i = 0;
                     i < command->data.leaderboard.top_player_count;
                     ++i) {
                    strcpy(
                        command->data.leaderboard.username_list
                        + i * USERNAME_MAX_SIZE,
                        leaderboard[i]._pseudo.c_str()
                    );
                    command->data.leaderboard.xps[i] = leaderboard[i]._xp;
                }
                // FIXME: there is no failed state?
                command->data.leaderboard.state =
                    Command::Leaderboard::State::Success;
                connection->getSendingCommandQueue()->push(command);
            }
            break;
        }
        case Command::Type::CreateLevel: {
            auto database = connection->getDatabase();
            std::string level_name = static_cast<std::string>(command->data.create_level.name_level);
            std::string author = static_cast<std::string>(command->data.create_level.username_creator);
            database->lock();
            if (!database->isLevelNameAlreadyTaken(level_name)){
                database->unlock();
                if (command->data.create_level.state != Command::CreateLevel::State::Check){
                    std::string filename = "levels/" + level_name;
                    std::ofstream file (filename);
                    file << command->data.create_level.level;
                    file.close();
                    if (!file){
                        command->data.create_level.state = Command::CreateLevel::State::Error;
                    }
                    else{
                        command->data.create_level.state = Command::CreateLevel::State::Created;
                        database->lock();
                        database->newLevel(level_name, author);
                        database->unlock();
                    }
                }else{
                    command->data.create_level.state = Command::CreateLevel::State::Name_ok;
                }
            }else{
                database->unlock();
                command->data.create_level.state = Command::CreateLevel::State::Doublon;
            }
            connection->getSendingCommandQueue()->push(command);
            break;
        }
        case Command::Type::UserLevelList: {
            auto database = connection->getDatabase();
            std::string author = static_cast<std::string>(command->data.user_level_list.username_creator);
            database->lock();
            auto levels = database->showLevels(author);
            database->unlock();
            command->data.user_level_list.nb_level =
                    levels.size() < LEVEL_MAX_COUNT ?
                    static_cast<unsigned>(levels.size()) :
                    LEVEL_MAX_COUNT;
            for (unsigned long i = 0; i < command->data.user_level_list.nb_level; ++i) {
                    (levels[i]._creator).erase(levels[i]._creator.end() - 4, levels[i]._creator.end());
                    strcpy(
                        command->data.user_level_list.level_list + i * USERNAME_MAX_SIZE,
                        levels[i]._creator.c_str()
                    );
                    command->data.user_level_list.level_like[i] = levels[i]._likes;
                }
            connection->getSendingCommandQueue()->push(command);
            break;
        }
        case Command::Type::LikeLevel: {
            auto database = connection->getDatabase();
            std::string liker = static_cast<std::string>(command->data.like_level.username_liker);
            std::string level = static_cast<std::string>(command->data.like_level.level_name);
            database->lock();
            database->newLike(level, liker);
            database->unlock();
            delete command;
            break;
        }
        case Command::Type::IsLiked : {
            auto database = connection->getDatabase();
            std::string liker = static_cast<std::string>(command->data.is_liked.username_liker);
            std::string level = static_cast<std::string>(command->data.is_liked.level_name);
            database->lock();
            if(database->likedBy(level, liker))
                command->data.is_liked.liked = true;
            database->unlock();
            connection->getSendingCommandQueue()->push(command);
            break;  
        }

#else // CLIENT v (Code de client commence ici)
#ifdef DEBUG
        case Command::Type::Ping: {
            command->data.ping.chrono.stop();
            DEBUGMSG("Ping: " <<
                              command->data.ping.chrono.getElapsedSecs() * 1000
                              << "ms");
            delete command;
            break;
        }
#endif // DEBUG
        case Command::Type::UpdateEntityPosition: {
            auto client     = connection->getOwner<Client::GameClient>();
            auto game_state = client->getGameState();
            game_state->updateEntityServerPosition(
                command->data.update_entity_position.id,
                command->data.update_entity_position.position
            );
            delete command;
            break;
        }
        case Command::Type::CreateEntity: {
            auto client     = connection->getOwner<Client::GameClient>();
            auto game_state = client->getGameState();
            GameEntity* new_entity;
            if(GameEntity::getGenericType(command->data.create_entity.type) == GenericType::Projectile)
                new_entity = GameEntity::from(
                    command->data.create_entity.type,
                    command->data.create_entity.id,
                    command->data.create_entity.position,
                    nullptr,
                    command->data.create_entity.is_hostile
                );
            else
                new_entity = GameEntity::from(
                    command->data.create_entity.type,
                    command->data.create_entity.id,
                    command->data.create_entity.position,
                    game_state
                );
            game_state->addEntity(new_entity);
            auto gameplay_window = client->getWindow()->getGameplayWindow();
            gameplay_window->addDrawable(new_entity);
            delete command;
            break;
        }
        case Command::Type::DestroyEntity: {
            auto client          = connection->getOwner<Client::GameClient>();
            auto game_state      = client->getGameState();
            auto gameplay_window = client->getWindow()->getGameplayWindow();
            gameplay_window->removeDrawable(command->data.destroy_entity.id);
            game_state->deleteEntity(command->data.destroy_entity.id);
            delete command;
            break;
        }
        case Command::Type::CreateAccount: {
            auto client = connection->getOwner<Client::GameClient>();
            if (command->data.create_account.state ==
                Command::CreateAccount::State::Success) {
                client->getWindow()->getWindowMenu()->setRegister(true);
            } else if (command->data.create_account.state ==
                       Command::CreateAccount::State::Failed) {
                client->getWindow()->getWindowMenu()->setRegister(false);
            }
            delete command;
            break;
        }
        case Command::Type::AuthLogin: {
            auto client = connection->getOwner<Client::GameClient>();
            if (command->data.auth_login.state ==
                Command::AuthLogin::State::Accepted) {
                client->getWindow()->getWindowMenu()->setLogin(true);
            } else if (command->data.auth_login.state ==
                       Command::AuthLogin::State::Denied) {
                client->getWindow()->getWindowMenu()->setLogin(false);
            }
            delete command;
            break;
        }
        case Command::Type::AuthRestore: {
            switch (command->data.auth_restore.state) {
                case Command::AuthRestore::State::ResponseSecretQuestion: {
                    auto client = connection->getOwner<Client::GameClient>();
                    client->getWindow()->getWindowMenu()->secretQuestion(
                        command->data.auth_restore.secret_question);
                    break;
                }
                case Command::AuthRestore::State::SecretAnswerAccepted: {
                    auto client = connection->getOwner<Client::GameClient>();
                    client->getWindow()->getWindowMenu()->secretAnswer(true);
                    break;
                }
                case Command::AuthRestore::State::SecretAnswerDenied: {
                    auto client = connection->getOwner<Client::GameClient>();
                    client->getWindow()->getWindowMenu()->secretAnswer(false);
                    break;
                }
                default: {
                    // we shouldn't be here
                    break;
                }
            }
            break;
        }
        case Command::Type::SendError: {
            auto client = connection->getOwner<Client::GameClient>();
            client->getWindow()->getWindowMenu()->setMenuError(
                strdup("PHRASE"));
            delete command;
            break;
        }
        case Command::Type::UserData: {
            auto client = connection->getOwner<Client::GameClient>();
            if (command->data.user_data.state ==
                Command::UserData::State::Completed) {
                switch (client->getWindow()->getWindowMenu()->getMenuType()) {
                    case MNAME::MY_PROFILE_MENU:
                        client->getWindow()->getWindowMenu()->setProfileUser(
                            command->data.user_data.xp,
                            command->data.user_data.secret_question,
                            command->data.user_data.secret_answer,
                            command->data.user_data.pending_friend_requests);
                        break;
                    case MNAME::LEADERBOARD_MENU:
                        client->getWindow()->getWindowMenu()->setMyLeaderboard(
                            command->data.user_data.xp);
                        break;
                    case MNAME::SOCIAL_MENU:
                        client->getWindow()->getWindowMenu()->findFriend(true);
                        break;
                    default:
                        break;
                }
            } else if (command->data.user_data.state ==
                       Command::UserData::State::NotFound) {
                client->getWindow()->getWindowMenu()->findFriend(false);
            }
            delete command;
            break;
        }
        case Command::Type::UserFriendList: {
            if (command->data.user_friendlist.state ==
                Command::UserFriendList::State::Completed) {
                auto client = connection->getOwner<Client::GameClient>();
                client->getWindow()->getWindowMenu()->socialMenuSetup(
                    command->data.user_friendlist.friend_list,
                    command->data.user_friendlist.friends_count);
                delete command;
            }
            break;
        }
        case Command::Type::FriendProfileInfo: {
            auto client = connection->getOwner<Client::GameClient>();
            switch (command->data.friend_profile.state) {
                case Command::FriendProfileInfo::State::FRIEND:
                    client->getWindow()->getWindowMenu()->setFriendInfo(
                        Menu::FRIEND_STATE::FRIEND,
                        command->data.friend_profile.score);
                    break;
                case Command::FriendProfileInfo::State::NOT_FRIEND:
                    client->getWindow()->getWindowMenu()->setFriendInfo(
                        Menu::FRIEND_STATE::NOT_FRIEND,
                        command->data.friend_profile.score);
                    break;
                case Command::FriendProfileInfo::State::PENDING:
                    client->getWindow()->getWindowMenu()->setFriendInfo(
                        Menu::FRIEND_STATE::ACCEPT,
                        command->data.friend_profile.score);
                    break;
                default:
                    break;
            }
            delete command;
            break;
        }
        case Command::Type::Leaderboard: {
            if (command->data.leaderboard.state ==
                Command::Leaderboard::State::Success) {
                auto client = connection->getOwner<Client::GameClient>();
                client->getWindow()->getWindowMenu()->setLeaderboard(
                    command->data.leaderboard.username_list,
                    command->data.leaderboard.xps);
            }
            delete command;
            break;
        }
        case Command::Type::GameStatus: {
            auto client = connection->getOwner<Client::GameClient>();
            if (command->data.game_status.state ==
                Command::GameStatus::State::Starts) {
                client->startGame(
                    command->data.game_status.username_player1,
                    command->data.game_status.username_player2,
                    command->data.game_status.settings
                );
                command->data.game_status.state =
                    Command::GameStatus::State::ClientInitialized;
                connection->getSendingCommandQueue()->push(command);
                client->getSound()->setLevelSound(1);
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::Ends) {
                client->endGame();
                client->getWindow()->getWindowMenu()->setEndMenu(
                    command->data.game_status.level_remain,
                    command->data.game_status.score_player1,
                    command->data.game_status.score_player2);
                client->getSound()->isWinner(command->data.game_status.level_remain<=0);
                delete command;
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::Pause) {
                auto game_state = client->getGameState();
                game_state->setState(GameState::State::Paused);
                game_state->updateHUDState();
                client->getSound()->isOnPause(true);
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::Resume) {
                auto game_state = client->getGameState();
                game_state->setState(GameState::State::Started);
                game_state->updateHUDState();
                client->getSound()->isOnPause(false);
            } else if (command->data.game_status.state ==
                       Command::GameStatus::State::Quit) {
                client->endGame();
                client->getSound()->stopMusic();
                delete command;
            }
            break;
        }
        case Command::Type::UpdateHUD: {
            auto client     = connection->getOwner<Client::GameClient>();
            auto game_state = client->getGameState();
            game_state->updateHUD(
                command->data.update_hud.username_player1,
                command->data.update_hud.username_player2,
                command->data.update_hud.player1_hp,
                command->data.update_hud.player1_score,
                command->data.update_hud.player2_hp,
                command->data.update_hud.player2_score,
                command->data.update_hud.boss_hp,
                command->data.update_hud.player_max_hp,
                command->data.update_hud.boss_max_hp
            );
            delete command;
            break;
        }
        case Command::Type::CreateLevel: {
            auto client     = connection->getOwner<Client::GameClient>();
            if (command->data.create_level.state == Command::CreateLevel::State::Created){
                client->getWindow()->getWindowMenu()->setLevelCreateMenu(true);
            }else if(command->data.create_level.state == Command::CreateLevel::State::Doublon){
                client->getWindow()->getWindowMenu()->setLevelChoiceNameMenu(false);
                client->getWindow()->getWindowMenu()->setLevelRoomMenu(false);
            }else if(command->data.create_level.state == Command::CreateLevel::State::Name_ok){
                client->getWindow()->getWindowMenu()->setLevelChoiceNameMenu(true);
                client->getWindow()->getWindowMenu()->setLevelRoomMenu(true);
            }else if(command->data.create_level.state == Command::CreateLevel::State::Error){
                client->getWindow()->getWindowMenu()->setLevelCreateMenu(false);
            }
            delete command;
            break;
        }
        case Command::Type::UserLevelList: {
            auto client     = connection->getOwner<Client::GameClient>();
            std::string author = static_cast<std::string>(command->data.user_level_list.username_creator);
            client->getWindow()->getWindowMenu()->friendLevelMenuSetup(
                    command->data.user_level_list.level_list,
                    command->data.user_level_list.level_like,
                    command->data.user_level_list.nb_level);
            delete command;
            break;
        }
        case Command::Type::IsLiked: {
            auto client     = connection->getOwner<Client::GameClient>();
            client->getWindow()->getWindowMenu()->setButtonLikeEndMenu(command->data.is_liked.liked);
            delete command;
            break;
        }
        
        case Command::Type::PlaySound: {
            auto client     = connection->getOwner<Client::GameClient>();
            if (command->data.play_sound.sound == Game::Client::SoundEffect::BossArrive) {
                client->getSound()->bossArriving(static_cast<int>(command->data.play_sound.level_number));
            } else if(command->data.play_sound.sound == Game::Client::SoundEffect::LevelChange) {
                client->getSound()->setLevelSound(static_cast<int>(command->data.play_sound.level_number));
            } else
                client->getSound()->playSoundEffect(command->data.play_sound.sound);
            delete command;
            break;
        }

        case Command::Type::Particle: {
            auto client     = connection->getOwner<Client::GameClient>();
            if(command->data.particle.type == SFML::Particle::DragonFire)
                dynamic_cast<Dragon*>(client->getGameState()->getBoss())->fireAnimation();
            else 
            	dynamic_cast<Game::Client::GraphicalWindow*>(client->getWindow())->getParticleHandler()->addParticle(command->data.particle.type, 
                                                                   command->data.particle.x, 
                                                                   command->data.particle.y,
                                                                   command->data.particle.w,
                                                                   command->data.particle.h,
                                                                   command->data.particle.reverse);
            delete command;
            break;
        }
#endif
        default: {
            break;
        }
    }
}
