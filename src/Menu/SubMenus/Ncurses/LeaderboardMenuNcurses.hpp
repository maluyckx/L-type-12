#ifndef LTYPE_MENU_LEADERBOARDMENUNCURSES_H
#define LTYPE_MENU_LEADERBOARDMENUNCURSES_H

#include "../../BaseMenuNcurses.hpp"
#include "../LeaderboardMenu.hpp"
#include <string>
#include <vector>
#include <iostream>

namespace Menu {
    class LeaderboardMenuNcurses : public BaseMenuNcurses, public LeaderboardMenu {
    public:
        LeaderboardMenuNcurses() noexcept;

        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;
        
        // LEADERBOARD SYSTEM
        void setLeaderboard(const char* usernames_list, int* scores_list) noexcept override;
        void setLeaderboardMy(int my_score) noexcept override;

        ~LeaderboardMenuNcurses() noexcept override =default;
    private:
        // LEADERBOARD DRAW
        void decorate() noexcept override;
        void createButton();
        void createWin();
        void createObjects();
        void cleanLeaderboard();
    };
}


#endif
