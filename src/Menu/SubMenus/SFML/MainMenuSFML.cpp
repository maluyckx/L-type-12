#include "MainMenuSFML.hpp"
#include <string>

Menu::MainMenuSFML::MainMenuSFML() noexcept {
    initAll();
}

void Menu::MainMenuSFML::initAll() noexcept {
    setFriendName("");
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2-225.f, 200.f, 450.f, 90.f, "sprites/button/simple_grey", "PLAY", 55, COLORS_SFML::YELLOW));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2-225.f, 300.f, 450.f, 90.f, "sprites/button/simple_grey", "LEVELS", 55, COLORS_SFML::YELLOW));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2-225.f, 410.f, 450.f, 90.f, "sprites/button/simple_grey", "SOCIAL", 55, COLORS_SFML::YELLOW));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2-225.f, 520.f, 450.f, 90.f, "sprites/button/simple_grey", "SCORES", 55, COLORS_SFML::YELLOW));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2-225.f, 630.f, 450.f, 90.f, "sprites/button/btn_red_", "EXIT", 55, COLORS_SFML::YELLOW));

    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2-32.f, 745.f, 64.f, 64.f, "sprites/button/btn_muted_", "", 55, COLORS_SFML::YELLOW, true));
    if (_client->getSound()->isMute())
        _buttons[5]->select();
    
    _sprites.emplace_back(new SFML::SpriteSimple(260.f, -25.f, 800.f, 250.f, "sprites/titles/l_animation/l_type_", 12));
}

MNAME Menu::MainMenuSFML::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (auto & _button : _buttons)
            _button->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (_buttons[0]->isClickOn())
            return MNAME::ROOM_MENU;
        else if (_buttons[1]->isClickOn())
            return MNAME::LEVEL_MAIN_MENU;
        else if (_buttons[2]->isClickOn())
            return MNAME::SOCIAL_MENU;
        else if (_buttons[3]->isClickOn())
            return MNAME::LEADERBOARD_MENU;
        else if (_buttons[4]->isClickOn())
            return MNAME::EXIT_MENU;
        else if (_buttons[5]->isClickOn()) {
            _client->getSound()->toggleMute();
        }
    }
    return MNAME::NOTHING;
}

void Menu::MainMenuSFML::animation() noexcept {
    Menu::BaseMenuSFML::animation();
    _sprites[0]->animate(0.2f);
}