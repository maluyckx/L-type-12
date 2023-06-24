#include "SocialMenu.hpp"
#include "../BaseMenu.hpp"

void Menu::SocialMenu::requestFriendList() {
    auto* command = Game::CommandFactory::userFriendList(MenuInfo::getUserName());
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}

 void Menu::SocialMenu::setFriendsList(const char* fl, unsigned count_friends) noexcept {
     for (size_t i = 0; i < count_friends; i++) {
        char tmp_str[32];
        int count_tmp=0;
        for (size_t j = i*32; j < 32*(i+1); j++) {
            tmp_str[count_tmp++] = fl[j];
        }
        _friends_list.emplace_back(tmp_str);
     }
 }
