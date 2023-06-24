#ifndef LTYPE_MENU_WIN_H
#define LTYPE_MENU_WIN_H

#include <stdlib.h>
#include <curses.h>
#include <string>
#include "Rectangle.hpp"

namespace Menu {
    /**
     * A simple Win object for ncurses
     */
    class Win {
        Rectangle _rect;
        WINDOW* _w;

        public:
        Win () noexcept = default;
        Win (int x, int y, int w, int h) noexcept: _rect(x, y , w, h), _w(nullptr)
            {_w = newwin(_rect.getHeight(), _rect.getWidth(), _rect.getPosY(), _rect.getPosX());}
        Win (WINDOW* wd, int x, int y, int w, int h) noexcept: _rect(x, y , w, h), _w(nullptr)
            {_w = subwin(wd, _rect.getHeight(), _rect.getWidth(), _rect.getPosY(), _rect.getPosX());}

        // Getter
        int getPosX() noexcept;
        int getPosY() noexcept;
        int getWidth() noexcept;
        int getHeight() noexcept;
        operator WINDOW* () noexcept;

        // destructeur
        ~Win() noexcept = default;

    }; 

}

#endif
