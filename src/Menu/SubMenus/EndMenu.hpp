#ifndef LTYPE_MENU_ENDMENU_H
#define LTYPE_MENU_ENDMENU_H

#include <string>
#include "../MenuInfo.hpp"

namespace Menu {

    class EndMenu: protected MenuInfo {
    protected:
        int _level_remain{};
        int _score_user{};
        int _score_friend{};
        bool _is_like=false;
        void logoutPlayer2(char* username);
        void quitSound() noexcept;
        bool isLevelACampaign();
    public:
        EndMenu() noexcept = default;
        EndMenu(int level_remain, int score_user, int score_friend) noexcept;
        virtual void setButtonLike(bool already_liked) = 0;
        ~EndMenu() noexcept override = default;
    };
}

#endif