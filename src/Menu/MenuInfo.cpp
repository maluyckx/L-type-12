#include "MenuInfo.hpp"

Game::Client::GameClient* Menu::MenuInfo::_client;
char Menu::MenuInfo::_user_name[32];
char Menu::MenuInfo::_friend_name[32];

void Menu::MenuInfo::setFriendName(std::string fn) noexcept {
    // Set friend name
    for (size_t i = 0; i < USERNAME_MAX_SIZE; i++) {
        if (i < fn.length())
            _friend_name[i] = fn[i];
        else
            _friend_name[i] = '\0';
    }
}