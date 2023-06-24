#ifndef LTYPE_MENU_BASEMENUNCURSES_H
#define LTYPE_MENU_BASEMENUNCURSES_H

#include "BaseMenu.hpp"
#include "Window/MenuMaker.hpp"
#include "../Game/Client/GameClient.hpp"
#include "../Game/Command.hpp"


namespace Menu {

    /**
     * Abstract menu with baics methode to use the Menu with POLYMORPHISME in Ncurses
     * herited from Ncurses. All Menus use MenuMaker, who's a simplification from
     * Ncurses lib
     * Like a lib
     * We're super proud of it :)
     */

    class BaseMenuNcurses: public BaseMenu {
    protected:
        Menu::MenuMaker* _window{};
        size_t  _choice=0;
        size_t _stars{};
        BaseMenuNcurses() noexcept = default;
        BaseMenuNcurses(const BaseMenuNcurses&) noexcept = default;
        BaseMenuNcurses& operator=(const BaseMenuNcurses&) noexcept = default;
    public:
        // initializes everything, window, buttons, Win and text sprite
        void initAll() noexcept override = 0;
        // Display everything each frame
        void display() override = 0;
        // Get inputs and return what he need to do
        MNAME inputs() override = 0;

        // Delete things
        void quitMenu() noexcept override;

        // Visual
        virtual void decorate() noexcept;
        void animation() noexcept override;

        ~BaseMenuNcurses() noexcept override { delete _window; };
    };
}

#endif