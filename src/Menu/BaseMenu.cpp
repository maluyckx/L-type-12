#include "BaseMenu.hpp"

void Menu::BaseMenu::timer(int ms) noexcept {
    // Good time fps manager for animation
    if (_timer.isTriggered(ms / 1000.)) {
        animation();
    }
}