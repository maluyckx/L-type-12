#include "ConnectionInfoNcurses.hpp"

bool Menu::ConnectionInfoNcurses::testValues() noexcept {
    for (size_t i = 0; i < _window->getNbInputText(); i++) {
        if (_window->getText(i)[MIN_TEXT-1] == '\0'){
            _window->changeObject(_window->getNbObject()-1, "All fields must be filled in with at least 3 characters.", 1);
            return false;
        }
    }
    return true;
}