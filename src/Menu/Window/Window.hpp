#ifndef LTYPE_MENU_WINDOW_HPP
#define LTYPE_MENU_WINDOW_HPP

#include <stdlib.h>
#include <curses.h>
#include <string>
#include <vector>
#include "TextInput.hpp"
#include "../../Utils/Constants.hpp"

enum SIZE{WIDTH=WIDTH_WINDOW, HEIGHT=HEIGHT_WINDOW};

namespace Menu {

    /**
     * Ncurses object
     */

    class Window {
    public:  
        Window () noexcept = default;

        // BASICS
        static void ncursesInit();
        static void ncursesStop();
        static void ncursesColors();
        // input
        static KEY input();
        // Utilities
        void update() const;
        static void clearAll();

        ~Window() noexcept = default;
    };

}

#endif