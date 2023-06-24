#include "LeaderboardMenu.hpp"
#include "../BaseMenu.hpp"

void Menu::LeaderboardMenu::requestLeaderboard() {
    auto* command = Game::CommandFactory::leaderboard();
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
    command = Game::CommandFactory::userData(MenuInfo::getUserName());
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}
