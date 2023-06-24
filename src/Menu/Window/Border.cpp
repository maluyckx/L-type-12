#include "Border.hpp"

Menu::Border::Border(WINDOW* wd, int x, int y, int w, int h) noexcept: _rect(x, y , w, h) {
    _w = subwin(wd, _rect.getHeight(), _rect.getWidth(), _rect.getPosY(), _rect.getPosX());
}

void Menu::Border::setHighlight() noexcept {
    _is_highlight = true;
}

void Menu::Border::unsetHighlight() noexcept {
    _is_highlight = false;
}

bool Menu::Border::isHighlight() const noexcept {
    return _is_highlight;
}

int Menu::Border::getPosX() noexcept {return _rect.getPosX();}
int Menu::Border::getPosY() noexcept {return _rect.getPosY();}
int Menu::Border::getWidth() noexcept {return _rect.getWidth();}
int Menu::Border::getHeight() noexcept {return _rect.getHeight();}