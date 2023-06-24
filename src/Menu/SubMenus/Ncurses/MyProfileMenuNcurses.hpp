#ifndef LTYPE_MENU_MYPROFILEMENUNCURSES_H
#define LTYPE_MENU_MYPROFILEMENUNCURSES_H

#include "ProfileMenuNcurses.hpp"
#include "../MyProfileMenu.hpp"
#include <string>


namespace Menu {
    class MyProfileMenuNcurses : public ProfileMenuNcurses, public MyProfileMenu {
    public:
        MyProfileMenuNcurses() noexcept;
        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;

        // REQUEST and Answer
        void setUser(int score, char* question, char* answer, const char* pending_list) override;

        ~MyProfileMenuNcurses() noexcept override = default;
    private:
        // Draw system
        MNAME goTo() noexcept;
        void createButton();
        void checkArrow();
        // Change info
        void changeInfo();
    };
}


#endif
