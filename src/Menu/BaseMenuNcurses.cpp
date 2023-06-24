#include "BaseMenuNcurses.hpp"

void Menu::BaseMenuNcurses::quitMenu() noexcept {
    //closes the window (and deletes the instance ?)
    delete _window;
    Menu::MenuMaker::ncursesStop();
}

void Menu::BaseMenuNcurses::decorate() noexcept {
    // Decoratation manager for ncurses Menu, draw stars in background
    _window->createWin(MAIN_WINDOW, 0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    _window->createObj(DECORATE_WINDOW, ".", 4, 3, 1);
    _window->createObj(DECORATE_WINDOW, ".", 12, 10, 1);
    _window->createObj(DECORATE_WINDOW, ".", 20, 30, 1);
    _window->createObj(DECORATE_WINDOW, ".", 110, 30, 1);
    _window->createObj(DECORATE_WINDOW, ".", 90, 25, 1);
    _window->createObj(DECORATE_WINDOW, ".", 108, 40, 1);
    _window->createObj(DECORATE_WINDOW, ".", 80, 39, 1);
    _window->createObj(DECORATE_WINDOW, ".", 60, 35, 1);
    _window->createObj(DECORATE_WINDOW, ".", 110, 3, 1);
    _window->createObj(DECORATE_WINDOW, ".", 101, 10, 1);
    _stars=10;
}

void Menu::BaseMenuNcurses::animation() noexcept {
    // Animated stars are always the last posObj to draw 
    for (size_t i = 0; i < _stars;++i) {
        _window->move(i, _window->getPos(i, true),
                    _window->getPos(i, false) + 1);
        if (_window->getPos(i, false) == SIZE::HEIGHT)
            _window->move(i, _window->getPos(i, true), 0);
    }
}
