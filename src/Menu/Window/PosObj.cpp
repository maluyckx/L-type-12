#include "PosObj.hpp"

int Menu::PosObj::getPosX() const noexcept {
    return _pos_x;
}

int Menu::PosObj::getPosY() noexcept {
    return _pos_y;
}

std::string Menu::PosObj::getText() noexcept {
    return _txt;
}

void Menu::PosObj::setPosX(int x) noexcept {
    _pos_x = x;
}

void Menu::PosObj::setPosY(int y) noexcept {
    _pos_y = y;
}

void Menu::PosObj::setColor(int c) noexcept {
    _color = c;
}

void Menu::PosObj::setText(std::string s) noexcept {
    _txt = s;
}

void Menu::PosObj::drawObject() {
    wattron(_w ,COLOR_PAIR(_color));
    int tmp_x = _pos_x;
    int tmp_y = _pos_y;
    for (char i : _txt){
        if (i != '\n'){
            mvwaddch(_w, tmp_y, tmp_x, static_cast<chtype>(i));
            tmp_x++;
        }
        else {
            tmp_x = _pos_x;
            tmp_y++;
        }
    }
    wattroff(_w ,COLOR_PAIR(_color));
}

void Menu::PosObj::clearObject() {
    wattron(_w ,COLOR_PAIR(_color));
    int tmp_x = _pos_x;
    int tmp_y = _pos_y;
    for (char i : _txt){
      mvwaddch(_w, tmp_y, tmp_x, ' ');
      tmp_x++;
      if (i == '\n'){
        tmp_x = _pos_x;
        tmp_y++;
      }
    }
    wattroff(_w ,COLOR_PAIR(_color));
}
