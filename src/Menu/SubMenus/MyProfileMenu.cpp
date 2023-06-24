#include "MyProfileMenu.hpp"
#include "../BaseMenu.hpp"


void Menu::MyProfileMenu::getUser() {
    auto* command = Game::CommandFactory::userData(MenuInfo::getUserName());
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
} 

void Menu::MyProfileMenu::changeInfo(char* infos) {
    auto* command = Game::CommandFactory::authRestore(
        Game::Command::AuthRestore::State::NewPassword
        ,MenuInfo::getUserName(),
        infos,
        strdup(""),
        strdup(""));
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}
