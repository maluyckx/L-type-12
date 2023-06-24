#ifndef LTYPE_MENU_SOCIALMENUNCURSES_H
#define LTYPE_MENU_SOCIALMENUNCURSES_H

#include "../../BaseMenuNcurses.hpp"
#include "../SocialMenu.hpp"
#include <random>
#include <string>

namespace Menu {
    class SocialMenuNcurses : public BaseMenuNcurses, public SocialMenu {
        int _ship_frame=0;
    public:
        SocialMenuNcurses() noexcept;

        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;

        void setFriendsList(const char* fl, unsigned count_friends) noexcept override;
        void setSearchText() override;

        ~SocialMenuNcurses() noexcept override =default;
    private:
        // Social
        void createButtons();

        // deco
        void checkArrow();
        void decorate() noexcept override;
        void animation() noexcept override;

        void searchFriend() override;

        MNAME goTo() noexcept;
    };
}

#endif
