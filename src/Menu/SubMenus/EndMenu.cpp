#include "EndMenu.hpp"
#include "../BaseMenu.hpp"
#include "RoomMenu.hpp"
#include "../../Utils/DebugLogger.hpp"

Menu::EndMenu::EndMenu(int level_remain, int score_user, int score_friend) noexcept:
                        _level_remain(level_remain), _score_user(score_user), _score_friend(score_friend) {}

void Menu::EndMenu::logoutPlayer2(char* username) {
    DEBUGMSG("=== LOGGING OUT (ENDMENU->LEADERBOARD): " << username);
    auto command = Game::CommandFactory::authLogout(username);
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}

void Menu::EndMenu::quitSound() noexcept {
    MenuInfo::getClient()->getSound()->stopMusic();
}

bool Menu::EndMenu::isLevelACampaign(){
    std::string l1 = "";
    std::string l2 = "";
    std::size_t i = 0;
    const char * level_played = RoomMenu::getLevelPlayed(); 
    const char * level_default = DEFAULT_LEVEL.c_str();
    while (level_played[i] != '\0'){
        l1 += level_played[i];
        i++;
    }
    
    std::size_t j = 0;
    while (level_default[j] != '\0'){
        l2 += level_default[j];
        j++;
    }
    l1.erase(l1.end() - 6, l1.end());
    l2.erase(l2.end() - 6, l2.end());
    if (l1 == l2) return true;
    return false;
}
