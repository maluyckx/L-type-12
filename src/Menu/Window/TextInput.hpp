#ifndef LTYPE_MENU_TEXTINPUT_H
#define LTYPE_MENU_TEXTINPUT_H

#include <string>
#include "Border.hpp"

enum class KEY{UP, DOWN, LEFT, RIGHT, ENTER, TAB, OTHER};
const int MIN_TEXT=3;

namespace Menu {
    /**
     * A simple Win object for ncurses
     */
    class TextInput : public Border {
        char* _txt{};  // Work with char cause ncurses use char* and not string :)
        size_t _txt_pos = 0; // Position in the text, next char write
        size_t _max_lenght{}; // Max lenght-1 of the text in the box
        bool _is_password{};

        public:
        TextInput () noexcept = default;
        TextInput (WINDOW* wd, size_t max, int x, int y, int w, int h, bool is_pass) noexcept;
        TextInput(const TextInput&) noexcept = default;
        TextInput& operator=(const Menu::TextInput&) noexcept = default;

        // BASICS
        void initTxtInput();
        KEY textWrite();
        static KEY getInput();
        bool checkChar(int ch);
        // Getter
        char* getTxt() noexcept {return _txt;}

        // destructeur
        ~TextInput() noexcept override = default;
    }; 

}

#endif
