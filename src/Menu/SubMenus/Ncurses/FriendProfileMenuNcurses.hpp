#ifndef LTYPE_MENU_FRIENDPROFILEMENUNCURSES_H
#define LTYPE_MENU_FRIENDPROFILEMENUNCURSES_H

#include "../FriendProfileMenu.hpp"
#include "ProfileMenuNcurses.hpp"

namespace Menu {
    class FriendProfileMenuNcurses : public ProfileMenuNcurses, public FriendProfileMenu {
        int _ship_frame=0;
    public:
        FriendProfileMenuNcurses() noexcept;
        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;

        void setFriendInfo(Menu::FRIEND_STATE state, int score) noexcept override;

        ~FriendProfileMenuNcurses() noexcept override = default;
    private:
        void animation() noexcept override;
        // Friends system
        void sendFriendRequest() override;
        void deleteFriend() override;
        void acceptFriendRequest() override;
        void rejectFriendRequest() override;
    };
}


#endif
