#ifndef LTYPE_MENU_POSOBJ_H
#define LTYPE_MENU_POSOBJ_H

#include <ncurses.h>
#include <string>

namespace Menu {
    /**
     * A simple Win object for ncurses
     */
    class PosObj {
        WINDOW* _w;
        int _pos_x;
        int _pos_y;
        std::string _txt;
        int _color;

        public:
        PosObj () noexcept = default;
        PosObj (WINDOW* w, int x, int y, std::string txt, int c) noexcept: _w(w), _pos_x(x), _pos_y(y), _txt(txt), _color(c) {}
        PosObj(const PosObj&) noexcept = default;
        PosObj& operator=(const PosObj&) noexcept = default;
        // Basics
        void drawObject();
        void clearObject();
        // Getter
        int getPosX() const noexcept;
        int getPosY() noexcept;
        std::string getText() noexcept;
        // Setter
        void setPosX(int x) noexcept;
        void setPosY(int y) noexcept;
        void setColor(int c) noexcept;
        void setText(std::string s) noexcept;

        // destructeur
        ~PosObj() noexcept = default;

    }; 

}

#endif
