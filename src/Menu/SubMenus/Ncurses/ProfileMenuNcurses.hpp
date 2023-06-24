#ifndef LTYPE_MENU_PROFILEMENUNCURSES_H
#define LTYPE_MENU_PROFILEMENUNCURSES_H

#include "../../BaseMenuNcurses.hpp"
#include <string>

namespace Menu {
    class ProfileMenuNcurses : public BaseMenuNcurses {
    public:
        ProfileMenuNcurses() noexcept=default;

        void initAll() noexcept override;
        void display() override;

        void decorate() noexcept override;

        ~ProfileMenuNcurses() noexcept override = default;
    };
}


#endif
