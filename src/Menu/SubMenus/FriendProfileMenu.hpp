#ifndef LTYPE_MENU_FRIENDPROFILEMENU_H
#define LTYPE_MENU_FRIENDPROFILEMENU_H

#include "../MenuInfo.hpp"
#include <string.h>

namespace Menu {

    enum FRIEND_STATE{NOT_FRIEND, FRIEND, ACCEPT};
    char* const FRIEND_TXT[3] = {strdup("ADD FRIEND"), strdup("DELETE FRIEND"), strdup("ACCEPT")};

    class FriendProfileMenu: protected MenuInfo {
        Menu::FRIEND_STATE _is_friend = Menu::FRIEND_STATE::NOT_FRIEND;
    public:
        FriendProfileMenu() noexcept = default;

        virtual void setFriendInfo(FRIEND_STATE state, int score) noexcept = 0;
        void setFriendState(Menu::FRIEND_STATE state) noexcept;
        Menu::FRIEND_STATE getFriendState() noexcept {return _is_friend;}

        ~FriendProfileMenu() noexcept override = default;
    protected:
        // Friends system
        virtual void sendFriendRequest();
        virtual void deleteFriend();
        virtual void acceptFriendRequest();
        virtual void rejectFriendRequest();
        void getFriendInfo();
    };
}


#endif
