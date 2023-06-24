#ifndef LTYPE_MENU_MYPROFILEMENU_H
#define LTYPE_MENU_MYPROFILEMENU_H

#include "../MenuInfo.hpp"
#include <string>
#include <vector>

namespace Menu {
    class MyProfileMenu: protected MenuInfo {
    protected:
        size_t _scrolling = 0;
        std::vector<std::string> _friends_waiting{};
    public:
        MyProfileMenu() noexcept = default;

        // REQUEST and Answer
        virtual void setUser(int score, char* question, char* answer, const char* pending_list) = 0;

        ~MyProfileMenu() noexcept override = default;
    protected:
        // Change info
        void getUser();
        void changeInfo(char* infos);
    };
}


#endif
