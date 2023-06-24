#ifndef LTYPE_MENU_HIGHLIGHT_HPP
#define LTYPE_MENU_HIGHLIGHT_HPP

#include <ncurses.h>
#include "Rectangle.hpp"

namespace Menu {

    class Border {
        Rectangle _rect{};
        bool _is_highlight=false;
    protected:
        WINDOW* _w{};
    public:  
        Border () noexcept = default;
        Border (WINDOW* wd, int x, int y, int w, int h) noexcept;
        Border(const Border& b) noexcept = default;
        Border& operator=(const Border&) noexcept = default;

        void setHighlight() noexcept;
        void unsetHighlight() noexcept;
        bool isHighlight() const noexcept;

        // getter
        int getPosX() noexcept;
        int getPosY() noexcept;
        int getWidth() noexcept;
        int getHeight() noexcept;
        // setter
        inline void setX(int x) noexcept {_rect.setPosX(x);}
        inline void setY(int y) noexcept {_rect.setPosY(y);}

        virtual ~Border() noexcept = default;
    };

}

#endif