#include "RoomMenu.hpp"
#include "../BaseMenu.hpp"
#include "../../Utils/DebugLogger.hpp"

char Menu::RoomMenu::_name_level[LEVEL_NAME_MAX_SIZE];

void Menu::RoomMenu::setLevelPlayed(std::string level) { 
    for (size_t i = 0; i < LEVEL_NAME_MAX_SIZE; i++) {
        if (i < level.length())
            _name_level[i] = level[i];
        else
            _name_level[i] = '\0';
    }   
}

void Menu::RoomMenu::start() {
    Game::GameSettings settings = Game::GameSettings(
        _is_solo,
        _difficulty,
        _dropping_rate,
        _friendly_fire,
        _lives,
        _level_name.c_str()
    );
    auto* command = Game::CommandFactory::gameStatus(
        Game::Command::GameStatus::State::Starts,
        settings,
        MenuInfo::getUserName(),
        MenuInfo::getFriendName(),
        0,
        0,
        false
    );
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}

void Menu::RoomMenu::logoutPlayer2(char* username) {
    DEBUGMSG("=== LOGGING OUT (ROOMENU): " << username);
    auto command = Game::CommandFactory::authLogout(username);
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}