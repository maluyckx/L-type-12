#include "LevelMainMenu.hpp"

Menu::LevelMainMenu::LevelMainMenu() noexcept {
    initAll();
}

void Menu::LevelMainMenu::initAll() noexcept {
    _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 230, 230, 510, 90, "sprites/button/simple_grey", "My Levels", 40, COLORS_SFML::YELLOW));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 230, 370, 500, 90, "sprites/button/simple_grey", "Create", 55, COLORS_SFML::YELLOW));
}

MNAME Menu::LevelMainMenu::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (auto & _button : _buttons)
            _button->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (_buttons[0]->isClickOn())
            return MNAME::MAIN_MENU;
        else if (_buttons[1]->isClickOn()){
            MenuInfo::setFriendName(MenuInfo::getUserName());
            return MNAME::FRIEND_LEVEL_MENU;
        }
        else if (_buttons[2]->isClickOn())
            return MNAME::LEVEL_CHOICE_SETTINGS_MENU;
    }
    return MNAME::NOTHING;
}

void Menu::LevelMainMenu::animation() noexcept {
    Menu::BaseMenuSFML::animation();
}