#ifndef LTYPE_MENU_LEADERBOARDMENU_H
#define LTYPE_MENU_LEADERBOARDMENU_H

#include "../MenuInfo.hpp"

namespace Menu {
    class LeaderboardMenu: protected MenuInfo {
    public:
        LeaderboardMenu() noexcept = default;
        
        // LEADERBOARD SYSTEM
        virtual void setLeaderboard(const char* usernames_list, int* scores_list) noexcept = 0;
        virtual void setLeaderboardMy(int my_score) noexcept = 0;

        ~LeaderboardMenu() noexcept override = default;
    protected:
        // REQUEST
        void requestLeaderboard();
    };
}


#endif
