#include "LoginMenuSFML.hpp"


Menu::LoginMenuSFML::LoginMenuSFML() noexcept {
    initAll();
}

void Menu::LoginMenuSFML::initAll() noexcept {
    _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 225.f, 450.f, 460.f, 92.f, "sprites/button/btn_blue_", "LOGIN", 55, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 275.f, 600.f, 550.f, 92.f, "sprites/button/btn_blue_", "RESET PASSWORD", 35, COLORS_SFML::WHITE));
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 375.f, 150.f, 750.f, 75.f, COLORS_SFML::WHITE, 37, COLORS_SFML::WHITE,19));
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 375.f, 300.f, 750.f, 75.f, COLORS_SFML::WHITE, 37, COLORS_SFML::WHITE,19, true));

    _textes.emplace_back(new SFML::Texte("", 180.f, 420.f, 17, COLORS_SFML::RED));
    _textes.emplace_back(new SFML::Texte("Username", 220.f, 110.f, 22, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Password", 220.f, 260.f, 22, COLORS_SFML::WHITE));
} 

MNAME Menu::LoginMenuSFML::windowInputs(sf::Event &event) noexcept {
    if(event.type == sf::Event::MouseMoved){
        for(size_t i = 0; i < _buttons.size(); i++)
            _buttons[i]->isOn(event.mouseMove.x,event.mouseMove.y);
        for (size_t i = 0; i < _txt_fields.size(); i++)
            _txt_fields[i]->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if(event.type == sf::Event::MouseButtonPressed){
        if (_buttons[0]->isClickOn())
            return MNAME::CONNECTION_MENU;
        else if(_buttons[1]->isClickOn())
            login();
        else if(_buttons[2]->isClickOn())
            return MNAME::USER_NAME_VERIFICATION_MENU; 

        for (size_t i = 0; i < _txt_fields.size(); ++i)
            _txt_fields[i]->isCLickOn();
    }
    if (event.type==sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter)
            login();
        else if (event.key.code == sf::Keyboard::Tab)
            switchField();
    } else if (event.type==sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            for (size_t i = 0; i < _txt_fields.size(); ++i) {
                _txt_fields[i]->update(static_cast<char>(event.text.unicode));
            }
        }
    }
    return MNAME::NOTHING;
}


 void Menu::LoginMenuSFML::login() {
     if (testValues()) {
        if (_is_user) {
            auto* command = Game::CommandFactory::authLogin(
                _txt_fields[0]->getTexte(),
                _txt_fields[1]->getTexte());
            _client->getConnection()->getSendingCommandQueue()->push(command);
            strcpy(_user_name, _txt_fields[0]->getTexte());
        }else if (strcmp(_txt_fields[0]->getTexte(), _user_name) != 0) {
            auto* command = Game::CommandFactory::authLogin(
                _txt_fields[0]->getTexte(),
                _txt_fields[1]->getTexte());
            _client->getConnection()->getSendingCommandQueue()->push(command);
            setFriendName(_txt_fields[0]->getTexte());
        }
        else 
            _textes[0]->setTexte("You can't play with you !");
    }else
        _textes[0]->setPosition(120.f, 425.f);
 }
 
 void Menu::LoginMenuSFML::setTextError() noexcept{
    _textes[0]->setPosition(220.f, 425.f);
    _textes[0]->setTexte("Bad username or password !");
 }