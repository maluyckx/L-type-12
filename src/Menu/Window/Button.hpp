#ifndef LTYPE_MENU_BUTTON_H
#define LTYPE_MENU_BUTTON_H

#include <string>
#include "Border.hpp"

namespace Menu {
    /**
     * A simple Win object for ncurses
     */
    class Button: public Border {
        std::string _name;
        bool _special = false;
        public:
        Button () noexcept = default;
        Button (WINDOW* wd, std::string n, int x, int y, int w, int h) noexcept:  Border(wd, x, y, w, h), _name(n) {}

        // BASICS
        void initButton();
        inline void setSpecial() {_special = true;}
        inline void unsetSpecial() {_special = false;}
        void move(WINDOW* wd);
        void changeTxt(char* new_txt) noexcept;
        // destructeur
        ~Button() noexcept override = default;

    }; 

}

#endif
