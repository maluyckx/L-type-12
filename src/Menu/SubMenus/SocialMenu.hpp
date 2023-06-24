#ifndef LTYPE_MENU_SOCIALMENU_H
#define LTYPE_MENU_SOCIALMENU_H

#include "../MenuInfo.hpp"
#include <random>
#include <string>

namespace Menu {
    class SocialMenu: protected MenuInfo {
    protected:
        size_t _scrolling = 0;
        std::vector<std::string> _friends_list{};
    public:
        SocialMenu() noexcept=default;

        virtual void setFriendsList(const char* fl, unsigned count_friends) noexcept;
        virtual void setSearchText() = 0;

        ~SocialMenu() noexcept override = default;
    protected:
        void requestFriendList();
        virtual void searchFriend() = 0;
    };
}

#endif
