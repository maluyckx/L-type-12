#include "ErrorMenuSFML.hpp"

Menu::ErrorMenuSFML::ErrorMenuSFML(char* str_erro) noexcept {
    strcpy(_str_error, str_erro);
    initAll();
}

void Menu::ErrorMenuSFML::initAll() noexcept {

}

MNAME Menu::ErrorMenuSFML::windowInputs(sf::Event &) noexcept {
    return MNAME::NOTHING;
}