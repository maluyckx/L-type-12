#include "RegisterMenuSFML.hpp"


Menu::RegisterMenuSFML::RegisterMenuSFML() noexcept {
    initAll();
}

void Menu::RegisterMenuSFML::initAll() noexcept {
    _buttons.emplace_back(new SFML::Button(50.f, HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 45, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 225.f, 660.f, 460.f, 92.f, "sprites/button/btn_blue_", "REGISTER", 45, COLORS_SFML::WHITE));
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 350.f, 100.f, 700.f, 60.f, COLORS_SFML::WHITE, 35, COLORS_SFML::WHITE,19));
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 350.f, 240.f, 700.f, 60.f, COLORS_SFML::WHITE, 35, COLORS_SFML::WHITE,19, true));
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 350.f, 380.f, 700.f, 60.f, COLORS_SFML::WHITE, 35, COLORS_SFML::WHITE,19));
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 350.f, 520.f, 700.f, 60.f, COLORS_SFML::WHITE, 35, COLORS_SFML::WHITE,19));
    _textes.emplace_back(new SFML::Texte("", 200.f, 620.f, 17, COLORS_SFML::RED));
    _textes.emplace_back(new SFML::Texte("Username", 220.f, 60.f, 20, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Password", 220.f, 200.f, 20, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Secret Question", 220.f, 340.f, 20, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Secret Asnwer", 220.f, 480.f, 20, COLORS_SFML::WHITE));
}


MNAME Menu::RegisterMenuSFML::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (auto & _button : _buttons)
            _button->isOn(event.mouseMove.x, event.mouseMove.y);
        for (auto & _txt_field : _txt_fields)
            _txt_field->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (_buttons[0]->isClickOn())
            return MNAME::CONNECTION_MENU;
        else if (_buttons[1]->isClickOn())
            createAccount();
        for (auto & _txt_field : _txt_fields)
            _txt_field->isCLickOn();
    }
    if (event.type==sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter)
            createAccount();
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

void Menu::RegisterMenuSFML::createAccount() {
    if (testValues()){
        auto* command = Game::CommandFactory::createAccount(
            _txt_fields[0]->getTexte(),
            _txt_fields[1]->getTexte(),
            _txt_fields[2]->getTexte(),
            _txt_fields[3]->getTexte());
        _client->getConnection()->getSendingCommandQueue()->push(command);
        if (_is_user)
            strcpy(_user_name, _txt_fields[0]->getTexte());
        else
            setFriendName(_txt_fields[0]->getTexte());
    } else
        _textes[0]->setPosition(120.f, 620.f);
}

 void Menu::RegisterMenuSFML::setTextError() noexcept {
    _textes[0]->setPosition(220.f, 620.f);
    _textes[0]->setTexte("This account already exists !");
 }