#include "NewPasswordMenuSFML.hpp"


Menu::NewPasswordMenuSFML::NewPasswordMenuSFML() noexcept {
    initAll();
}

void Menu::NewPasswordMenuSFML::initAll() noexcept {
    if (_background==nullptr) {
        _background = new SFML::Background("sprites/background/background_1");
        _stars = new SFML::Stars("sprites/background/stars_");
    }
    _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 225.f, 500.f, 460.f, 92.f, "sprites/button/btn_blue_", "DONE", 45.f, COLORS_SFML::WHITE));
    
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 375.f, 150.f, 750.f, 75.f, COLORS_SFML::WHITE, 37, COLORS_SFML::WHITE,19, true));
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 375.f, 300.f, 750.f, 75.f, COLORS_SFML::WHITE, 37, COLORS_SFML::WHITE,19, true));

    _textes.emplace_back(new SFML::Texte("", 180.f, 450.f, 19, COLORS_SFML::RED));
    _textes.emplace_back(new SFML::Texte("New password", 220.f, 110.f, 22, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Re-type password", 220.f, 260.f, 22, COLORS_SFML::WHITE));
}


MNAME Menu::NewPasswordMenuSFML::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (auto & _button : _buttons)
            _button->isOn(event.mouseMove.x, event.mouseMove.y);
        for (auto & _txt_field : _txt_fields)
            _txt_field->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        for (auto & _txt_field : _txt_fields)
            _txt_field->isCLickOn();
        if (_buttons[0]->isClickOn())
            return MNAME::LOGIN_MENU;
        else if (_buttons[1]->isClickOn())
            return setPassword();
    }
    if (event.type==sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter)
            return setPassword();
        else if (event.key.code == sf::Keyboard::Tab)
            switchField();
    } else if (event.type==sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            for (auto & _txt_field : _txt_fields) {
                _txt_field->update(static_cast<char>(event.text.unicode));
            }
        }
    }
    return MNAME::NOTHING;
}

bool Menu::NewPasswordMenuSFML::testPassword() noexcept {
    if ((strcmp(_txt_fields[0]->getTexte(), _txt_fields[1]->getTexte()) == 0))
        return true;
    _textes[0]->setTexte("The two passwords are different !");
    return false;
}

MNAME Menu::NewPasswordMenuSFML::setPassword() noexcept {
     if (testValues() && testPassword()){
        auto* command = Game::CommandFactory::authRestore(
            Game::Command::AuthRestore::State::NewPassword
            ,_user_name,
            _txt_fields[0]->getTexte(),
            strdup(""),
            strdup(""));
        _client->getConnection()->getSendingCommandQueue()->push(command);
        return MNAME::LOGIN_MENU;
     }  
     return MNAME::NOTHING;
}

 void Menu::NewPasswordMenuSFML::setTextError() noexcept {
    _textes[0]->setPosition(220.f, 425.f);
    _textes[0]->setTexte("Incorrect password !");
 }