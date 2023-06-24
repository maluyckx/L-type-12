#include "FriendProfileMenu.hpp"
#include "../BaseMenu.hpp"


void Menu::FriendProfileMenu::setFriendState(Menu::FRIEND_STATE state) noexcept {
    _is_friend=state;
}

void Menu::FriendProfileMenu::sendFriendRequest() {
    auto* command = Game::CommandFactory::friendRequest(
        Game::Command::FriendRequest::State::Pending
        ,MenuInfo::getUserName(),
         MenuInfo::getFriendName());
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}

void Menu::FriendProfileMenu::deleteFriend() {
    auto* command = Game::CommandFactory::friendRequest(
        Game::Command::FriendRequest::State::Delete,
        MenuInfo::getUserName(),
        MenuInfo::getFriendName());
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
    _is_friend=Menu::FRIEND_STATE::NOT_FRIEND;
}

void Menu::FriendProfileMenu::acceptFriendRequest() {
    auto* command = Game::CommandFactory::friendRequest(
        Game::Command::FriendRequest::State::Accepted
        ,MenuInfo::getUserName(),
         MenuInfo::getFriendName());
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
    _is_friend=Menu::FRIEND_STATE::FRIEND;
}

void Menu::FriendProfileMenu::rejectFriendRequest() {
    auto* command = Game::CommandFactory::friendRequest(
        Game::Command::FriendRequest::State::Denied
        ,MenuInfo::getUserName(),
         MenuInfo::getFriendName());
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
    _is_friend=Menu::FRIEND_STATE::NOT_FRIEND;
}

void Menu::FriendProfileMenu::getFriendInfo() {
    // Get les infos du joueur
    auto* command = Game::CommandFactory::friendProfileInfo(MenuInfo::getFriendName(), MenuInfo::getUserName());
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}