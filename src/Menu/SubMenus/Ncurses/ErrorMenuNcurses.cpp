#include "ErrorMenuNcurses.hpp"

Menu::ErrorMenuNcurses::ErrorMenuNcurses(char* str_erro) noexcept {
    strcpy(_str_error, str_erro);
    initAll();
}

void Menu::ErrorMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    Menu::MenuMaker::clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    _window->createButton("QUIT", 50, 20, 30, 5);
    _window->createObj(MAIN_WINDOW, "Error :", SIZE::WIDTH/2 - 4, 13, 1);
    _window->createObj(MAIN_WINDOW, _str_error,static_cast<int>(SIZE::WIDTH/2 - strlen(_str_error)/2), 15, 1);
}

void Menu::ErrorMenuNcurses::display(){
    timer(50);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawBox(MAIN_WINDOW, 0);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::ErrorMenuNcurses::inputs() {
     _window->unsetHiglight(_choice);
    switch (Menu::MenuMaker::input()) {
        case KEY::ENTER:
            return MNAME::EXIT_MENU; 
        break;
        default:
            break;
    }
    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

void Menu::ErrorMenuNcurses::decorate() noexcept {
    Menu::BaseMenuNcurses::decorate();

}

void Menu::ErrorMenuNcurses::animation() noexcept {
    Menu::BaseMenuNcurses::animation();
}