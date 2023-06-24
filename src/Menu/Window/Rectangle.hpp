#ifndef LTYPE_MENU_RECTANGLE_H
#define LTYPE_MENU_RECTANGLE_H

namespace Menu {
    /**
     * A rectangel
     */
    class Rectangle {
        int _pos_x;
        int _pos_y;
        int _width;
        int _height;

        public:
        Rectangle () noexcept = default;
        Rectangle (int x, int y, int w, int h) noexcept: _pos_x(x), _pos_y(y), _width(w), _height(h) {}

        // Setter
        inline void setPosX(int x) noexcept {_pos_x=x;}
        inline void setPosY(int y) noexcept {_pos_y=y;}
        inline void setWidth(int w) noexcept {_width=w;}
        inline void setHeight(int h) noexcept {_height=h;}
        // Getter
        int getPosX() const noexcept {return _pos_x;}
        int getPosY() const noexcept {return _pos_y;}
        int getWidth() const noexcept {return _width;}
        int getHeight() const noexcept {return _height;}

        // destructeur
        ~Rectangle() noexcept = default;

    }; 

}

#endif
