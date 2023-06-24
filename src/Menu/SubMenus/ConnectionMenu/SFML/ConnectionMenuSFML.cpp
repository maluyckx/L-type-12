#include "ConnectionMenuSFML.hpp"
#include <string>
#include "../ConnectionInfo.hpp"

Menu::ConnectionMenuSFML::ConnectionMenuSFML(Game::Client::GameClient* client) noexcept {
    initAll();
    MenuInfo::setClient(client);
}

void Menu::ConnectionMenuSFML::initAll() noexcept {
    if (_background==nullptr) {
        _background = new SFML::Background("sprites/background/background_1");
        _stars = new SFML::Stars("sprites/background/stars_");
    }
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 225.f, 200.f, 460.f, 92.f, "sprites/button/btn_blue_", "LOGIN", 45.f, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 225.f, 450.f, 460.f, 92.f, "sprites/button/btn_blue_", "REGISTER", 45.f, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(50.f, HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "EXIT", 45.f, COLORS_SFML::WHITE));

    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 32.f, 600, 64.f, 64.f, "sprites/button/btn_muted_", "", 45.f, COLORS_SFML::WHITE, true));
    if (_client != nullptr && _client->getSound() != nullptr && _client->getSound()->isMute())
        _buttons[3]->select();
}

void Menu::ConnectionMenuSFML::display(){
    Menu::BaseMenuSFML::display();
}

MNAME Menu::ConnectionMenuSFML::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (auto & _button : _buttons)
            _button->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (_buttons[0]->isClickOn())
            return MNAME::LOGIN_MENU;
        else if (_buttons[1]->isClickOn())
            return MNAME::REGISTER_MENU;
        else if (_buttons[2]->isClickOn()) {
            if (Menu::ConnectionInfo::_is_user)
                return MNAME::EXIT_MENU;
            return MNAME::ROOM_MENU;
        }
        else if (_buttons[3]->isClickOn()) {
            _client->getSound()->toggleMute();
        }
    }
    return MNAME::NOTHING;
}

void Menu::ConnectionMenuSFML::animation() noexcept {
    Menu::BaseMenuSFML::animation();
}