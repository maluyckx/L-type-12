#ifndef LTYPE_MENU_MAINMENUNCURSES_H
#define LTYPE_MENU_MAINMENUNCURSES_H

#include "../../BaseMenuNcurses.hpp"
#include "../../MenuInfo.hpp"
#include <string>
#include <vector>
#include <iostream>

namespace Menu {

    class MainMenuNcurses : public BaseMenuNcurses, protected MenuInfo {
    public:
        MainMenuNcurses() noexcept;

        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;

        ~MainMenuNcurses() noexcept override = default;
    };
}

#endif
