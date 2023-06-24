#include "TextInput.hpp"

static const std::string CORRECT_CHAR = " &*^%$#@+-_"; // Special char authorized
const int KEY_BACKSPACE_MAC_OS = 127;

Menu::TextInput::TextInput(WINDOW* wd, size_t max, int x, int y,
                           int w, int h, bool is_pass) noexcept:
        Border(wd, x, y, w, h), _max_lenght(max), _is_password(is_pass) {
    _txt = static_cast<char*>(malloc(sizeof(char) * (_max_lenght)));
    for (size_t i = 0; i < _max_lenght; i++) {
        _txt[i] = '\0';
    }
}

void Menu::TextInput::initTxtInput() {
    if (isHighlight())
        wattron(_w, A_REVERSE);
    box(_w, 0, 0);
    wattroff(_w, A_REVERSE);
    for (size_t i = 0; i < _max_lenght; i++) {
        if (_is_password){
            if (_txt[i] != '\0')
                mvwaddch(_w, getHeight() / 2, static_cast<int>(2 + i),
                    static_cast<chtype>('*'));
            else
                mvwaddch(_w, getHeight() / 2, static_cast<int>(2 + i),
                    static_cast<chtype>(' '));
        }
        else{
            if (_txt[i] != '\0')
                mvwaddch(_w, getHeight() / 2, static_cast<int>(2 + i),
                        static_cast<chtype>(_txt[i]));
            else
                mvwaddch(_w, getHeight() / 2, static_cast<int>(2 + i),
                        static_cast<chtype>(' '));
        }
    }
    if (isHighlight())
        mvwaddch(_w, getHeight() / 2, static_cast<int>(2 + _txt_pos),
                 ACS_BULLET);
    wrefresh(_w);
}

KEY Menu::TextInput::textWrite() {
    int ch = getch();
    KEY tmp;
    switch (ch) {
        case KEY_BACKSPACE:
        case KEY_BACKSPACE_MAC_OS:
            tmp = KEY::OTHER;
            if (_txt_pos > 0) {
                _txt_pos--;
                _txt[_txt_pos] = '\0';
            }
            break;
        case KEY_UP:
            tmp = KEY::UP;
            break;
        case KEY_DOWN:
            tmp = KEY::DOWN;
            break;
        case KEY_LEFT:
            tmp = KEY::LEFT;
            break;
        case KEY_RIGHT:
            tmp = KEY::RIGHT;
            break;
        case '\t':
            tmp = KEY::TAB;
            break;
        default:
            tmp = KEY::OTHER;
            if (_txt_pos < _max_lenght - 1 && checkChar(ch)) {
                _txt[_txt_pos] = static_cast<char>(ch);
                _txt_pos++;
            }

            break;
    }
    return tmp;
}

KEY Menu::TextInput::getInput() {
    switch (getch()) {
        case KEY_UP:
            return KEY::UP;
            break;
        case KEY_DOWN:
            return KEY::DOWN;
            break;
        case KEY_LEFT:
            return KEY::LEFT;
            break;
        case KEY_RIGHT:
            return KEY::RIGHT;
            break;
         case 10:
            return KEY::ENTER;
            break;
        default:
            return KEY::OTHER;
            break;        
        case 'z':
            return KEY::UP;
            break;
        case 's':
            return KEY::DOWN;
            break;
        case 'q':
            return KEY::LEFT;
            break;
        case 'd':
            return KEY::RIGHT;
            break;
    }
}

bool Menu::TextInput::checkChar(int ch) {
    if (isalnum(ch) || CORRECT_CHAR.find(static_cast<char>(ch)) != std::string::npos)
        return true;
    return false;
}