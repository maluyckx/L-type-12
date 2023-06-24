#include "Win.hpp"

int Menu::Win::getPosX() noexcept {return _rect.getPosX();}

int Menu::Win::getPosY() noexcept {return _rect.getPosY();}

int Menu::Win::getWidth() noexcept {return _rect.getWidth();}

int Menu::Win::getHeight() noexcept {return _rect.getHeight();}

Menu::Win::operator WINDOW* () noexcept{
    return _w;
}