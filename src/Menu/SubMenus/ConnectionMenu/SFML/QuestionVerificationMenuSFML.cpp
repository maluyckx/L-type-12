#include "QuestionVerificationMenuSFML.hpp"


Menu::QuestionVerificationMenuSFML::QuestionVerificationMenuSFML(char* question_str) noexcept {
    strcpy(_question, question_str);
    initAll();
}

void Menu::QuestionVerificationMenuSFML::initAll() noexcept {
	_buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50.f, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 225.f,  450.f,  460.f, 92.f, "sprites/button/btn_blue_", "DONE", 55.f, COLORS_SFML::WHITE));
    _txt_fields.emplace_back(new SFML::TexteField(WIDTH_WINDOW_SFML/2 - 225.f, 300.f, 460.f, 92.f, COLORS_SFML::WHITE, 45.f, COLORS_SFML::WHITE,16));
    _textes.emplace_back(new SFML::Texte("", 180.f, 420.f, 17, COLORS_SFML::RED));
    _textes.emplace_back(new SFML::Texte("Answer your secret question !", WIDTH_WINDOW_SFML/2 - 250.f, 200.f, 22, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte(_question, WIDTH_WINDOW_SFML/2 - 250.f, 240.f, 22, COLORS_SFML::WHITE));
}


MNAME Menu::QuestionVerificationMenuSFML::windowInputs(sf::Event &event) noexcept {
	if(event.type == sf::Event::MouseMoved){
        for(auto & _button : _buttons)
            _button->isOn(event.mouseMove.x,event.mouseMove.y);
        _txt_fields[0]->isOn(event.mouseMove.x,event.mouseMove.y);
    }
    if(event.type == sf::Event::MouseButtonPressed){
        if (_buttons[0]->isClickOn())
            return MNAME::LOGIN_MENU;
        else if(_buttons[1]->isClickOn())
            return testQuestion();
        for (auto & _txt_field : _txt_fields)
            _txt_field->isCLickOn();
    }
    if (event.type==sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter)
            testQuestion();
    } else if (event.type==sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            for (auto & _txt_field : _txt_fields) {
                _txt_field->update(static_cast<char>(event.text.unicode));
            }
        }
    }
    return MNAME::NOTHING;

}

MNAME Menu::QuestionVerificationMenuSFML::testQuestion() noexcept {
	if (testValues()){
        auto* command = Game::CommandFactory::authRestore(
            Game::Command::AuthRestore::State::CheckSecretAnswer,
            _user_name,
            strdup(""),
            _question,
            _txt_fields[0]->getTexte());
        _client->getConnection()->getSendingCommandQueue()->push(command);
     }
     return MNAME::NOTHING;
 }

 void Menu::QuestionVerificationMenuSFML::setTextError() noexcept {
     _textes[0]->setPosition(WIDTH_WINDOW_SFML/2 - 100.f, 420.f);
     _textes[0]->setTexte("Bad response!");
 }