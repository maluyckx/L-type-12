#include "ProfileMenuNcurses.hpp"

const char* SPRITE_PLANET = "                     .::.\n"
"                  .:'  .:\n"
"        ,MMM8&&&.:'   .:'\n"
"       MMMMM88&&&&  .:'\n"
"      MMMMM88&&&&&&:'\n"
"      MMMMM88&&&&&&\n"
"    .:MMMMM88&&&&&&\n"
"  .:'  MMMMM88&&&&\n"
".:'   .:'MMM8&&&'\n"
":'  .:'\n"
"'::'";

void Menu::ProfileMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);    
    decorate();
    _window->createButton("BACK", 3, 35, 10, 5);
    _window->createObj(MAIN_WINDOW, "SCORE : 121232", SIZE::WIDTH/2 - 30 /2, 7, 1);
}

void Menu::ProfileMenuNcurses::display() {
    timer(69);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawBox(MAIN_WINDOW, 1);
}

void Menu::ProfileMenuNcurses::decorate() noexcept {
    _window->createWin(MAIN_WINDOW, 0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    _window->createObj(DECORATE_WINDOW, ".", 4, 3, 1);
    _window->createObj(DECORATE_WINDOW, ".", 6, 20, 1);
    _window->createObj(DECORATE_WINDOW, ".", 8, 10, 1);
    _window->createObj(DECORATE_WINDOW, ".", 30, 2, 1);
    _window->createObj(DECORATE_WINDOW, ".", 10, 32, 1);
    _window->createObj(DECORATE_WINDOW, ".", 20, 18, 1);
    _window->createObj(DECORATE_WINDOW, ".", 32, 27, 1);
    _window->createObj(DECORATE_WINDOW, ".", 13, 23, 1);
    _stars=8;
    _window->createObj(DECORATE_WINDOW, SPRITE_PLANET, 5, 3, 1);
}