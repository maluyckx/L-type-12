#include "Button.hpp"

void Menu::Button::initButton() {
    if (isHighlight())
        wattron(_w, A_REVERSE);
    box(_w, 0, 0);
    if (!_special)
        wattroff(_w, A_REVERSE);
    char* tmp;
    tmp = &_name[0];
    mvwprintw(_w, getHeight()/2, (getWidth()/2)-static_cast<int>(_name.length()/2), tmp);
    wattroff(_w, A_REVERSE);
    wrefresh(_w);
}

void Menu::Button::move(WINDOW* wd) {
    wclear(_w);
    _w = subwin(wd, getHeight(), getWidth(), getPosY(), getPosX());
}

void Menu::Button::changeTxt(char* new_txt) noexcept {
    wclear(_w);
    _name = new_txt;
}