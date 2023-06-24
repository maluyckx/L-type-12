#ifndef LTYPE_MENU_MENUMAKER_HPP
#define LTYPE_MENU_MENUMAKER_HPP

#include <stdlib.h>
#include <curses.h>
#include <string>
#include <vector>
#include "Window.hpp"
#include "Button.hpp"
#include "Win.hpp"
#include "PosObj.hpp"

const size_t MAIN_WINDOW=0;
const size_t DECORATE_WINDOW=1;

namespace Menu {

    /**
     * Ncurses Menu Maker
     */

    class MenuMaker: public Window {
        std::vector<Menu::Button> _btn{};
        std::vector<Menu::Win> _win{};
        std::vector<Menu::TextInput> _txt_input{};
        std::vector<Menu::PosObj> _obj{};

    public:  
        MenuMaker () noexcept = default;

        // Create
        void createWin(int x, int y, int w, int h);
        void createWin(size_t who, int x, int y, int w, int h);
        void createButton(std::string n, int x, int y, int w, int h);
        void createTxtInput(size_t max, int x, int y, int w, int h, bool is_password=false);
        void createObj(size_t who, const std::string& txt, int pos_x, int pos_y, int color);
        // Input text actions
        KEY writeText(size_t who);
        char* getText(size_t who);
        size_t getNbInputText() {return _txt_input.size();}
        // Button actions
        size_t getNbButtons() {return _btn.size();}
        void scrollButtons(size_t min, size_t max, bool direction);
        void changeButtons(size_t who, char* new_txt);
        // highlight
        void setHiglight(size_t who, bool special=false) noexcept;
        void unsetHiglight(size_t who, bool special=false) noexcept;
        // Object
        void changeObject(size_t who, std::string s, int c) noexcept;
        void move(size_t who, int x, int y) noexcept;
        int getPos(size_t who, bool xy) noexcept;
        size_t getNbObject() {return _obj.size();};
        // Draw
        void drawButtons();
        void drawTxtInput();
        void drawObjects();
        void drawBox(size_t who, int color);
        // Utilities
        void updateWin(size_t who);

        ~MenuMaker() noexcept = default;
    };

}

#endif