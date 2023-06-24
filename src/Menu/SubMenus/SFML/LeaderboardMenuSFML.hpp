#ifndef LTYPE_MENU_LEADERBOARDMENUSFML_H
#define LTYPE_MENU_LEADERBOARDMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../LeaderboardMenu.hpp"
#include <string>
#include <vector>
#include <iostream>

namespace Menu {
    class LeaderboardMenuSFML : public BaseMenuSFML, public LeaderboardMenu {
    public:
        LeaderboardMenuSFML() noexcept;
        // BASICS
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        void animation() noexcept override;
        // LEADERBOARD SYSTEM
        void setLeaderboard(const char* usernames_list, int* scores_list) noexcept override;
        void setLeaderboardMy(int my_score) noexcept override;

        ~LeaderboardMenuSFML() noexcept override = default;
    };
}


#endif
