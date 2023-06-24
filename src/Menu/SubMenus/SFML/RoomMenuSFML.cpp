#include "RoomMenuSFML.hpp"
#include "../ConnectionMenu/ConnectionInfo.hpp"

Menu::RoomMenuSFML::RoomMenuSFML() noexcept {
    initAll();
}

void Menu::RoomMenuSFML::initAll() noexcept {
    _buttons.emplace_back(new SFML::Button(335.f, 180.f, 350.f, 80.f, "sprites/button/btn_select_", "PLAYER 2", 38, COLORS_SFML::YELLOW, true));

    _buttons.emplace_back(new SFML::Button(335.f, 280.f, 275.f, 70.f, "sprites/button/btn_select_", "EASY", 35, COLORS_SFML::YELLOW, true));
    _buttons.emplace_back(new SFML::Button(630.f, 280.f, 275.f, 70.f, "sprites/button/btn_select_", "NORMAL", 35, COLORS_SFML::YELLOW, true));
    _buttons.emplace_back(new SFML::Button(935.f, 280.f, 275.f, 70.f, "sprites/button/btn_select_", "HARD", 35, COLORS_SFML::YELLOW, true));

    _buttons.emplace_back(new SFML::Button(335.f, 380.f, 275.f, 70.f, "sprites/button/btn_select_", "A LOT", 35, COLORS_SFML::YELLOW, true));
    _buttons.emplace_back(new SFML::Button(630.f, 380.f, 275.f, 70.f, "sprites/button/btn_select_", "NORMAL", 35, COLORS_SFML::YELLOW, true));
    _buttons.emplace_back(new SFML::Button(935.f, 380.f, 275.f, 70.f, "sprites/button/btn_select_", "NONE", 35, COLORS_SFML::YELLOW, true));

    _buttons.emplace_back(new SFML::Button(515.f, 480.f, 275.f, 70.f, "sprites/button/btn_select_", "NO", 35, COLORS_SFML::YELLOW, true));
    _buttons.emplace_back(new SFML::Button(795.f, 480.f, 275.f, 70.f, "sprites/button/btn_select_", "YES", 35, COLORS_SFML::YELLOW, true));
    
    _buttons.emplace_back(new SFML::Button(335.f, 580.f, 275.f, 70.f, "sprites/button/btn_select_", "3", 35, COLORS_SFML::YELLOW, true));
    _buttons.emplace_back(new SFML::Button(630.f, 580.f, 275.f, 70.f, "sprites/button/btn_select_", "5", 35, COLORS_SFML::YELLOW, true));
    _buttons.emplace_back(new SFML::Button(935.f, 580.f, 275.f, 70.f, "sprites/button/btn_select_", "10", 35, COLORS_SFML::YELLOW, true));
    
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 220, 680.f, 450, 90, "sprites/button/simple_grey", "PLAY", 55, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));
    
    _buttons.emplace_back(new SFML::Button(810.f, 120.f, 60, 60, "sprites/button/button_send_", "")); // Bouton check 
    _buttons.emplace_back(new SFML::Button(810.f, 40.f, 270, 70, "sprites/button/simple_grey", "Campaign", 30, COLORS_SFML::YELLOW)); // Button load campagne
    
    _txt_fields.emplace_back(new SFML::TexteField(310.f, 80.f, 450.f, 80.f, COLORS_SFML::YELLOW, 30, COLORS_SFML::YELLOW, 14));

    std::string level_name_create;
    if(*getLevelPlayed() != '\0'){
        level_name_create = getLevelPlayed();
        level_name_create.erase(level_name_create.end() - 4, level_name_create.end());
    }else{
        RoomMenu::setLevelPlayed(_level_name);
    }
    _textes.emplace_back(new SFML::Texte(_friend_name, 710.f, 195.f, 30, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte(level_name_create, 900.f, 142.f, 20, COLORS_SFML::YELLOW));
    _textes.emplace_back(new SFML::Texte("COOP"      , 30.f, 190.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("DIFFICULTY", 30.f, 290.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("DROP RATE" , 30.f, 390.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("FRIENDLY FIRE", 30.f,490.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("LIVES"     , 30.f, 590.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("LEVEL SELECT", 30.f, 100.f, 22, COLORS_SFML::WHITE));
    startHiglight();
}

MNAME Menu::RoomMenuSFML::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (auto & _button : _buttons)
            _button->isOn(event.mouseMove.x, event.mouseMove.y);
        for (auto & _txt_field : _txt_fields)
            _txt_field->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        _txt_fields[0]->isCLickOn();
        if (_buttons[0]->isClickOn()){
            if (_friend_name[0]=='\0') {
                Menu::ConnectionInfo::_is_user = false;
                return MNAME::CONNECTION_MENU;
            }
            logoutPlayer2(getFriendName());
            _is_solo=Game::GameSettings::GameMode::Single;
            setFriendName(strdup("\0"));
            _textes[0]->setTexte(" ");
            return MNAME::NOTHING;
        } else if (_buttons[12]->isClickOn()) { // Bouton play
            if (_level_name_correct){
                if (_name_level != _level_name && _name_level == strdup("")){
                    RoomMenu::setLevelPlayed(_level_name);
                }else{
                    _level_name = _name_level;
                }
                _level_name = "levels/" + _level_name;
                if (_friend_name[0]!='\0')
                    _is_solo = Game::GameSettings::GameMode::Coop;
                start();
                return MNAME::GAME;
            }else{
                _textes[1]->setTexte("Enter name of level");
            }
            return MNAME::NOTHING;
        }
        else if (_buttons[13]->isClickOn()) {
            logoutPlayer2(getFriendName());
            return MNAME::MAIN_MENU;
        }
        else if (_buttons[14]->isClickOn()) {
            _level_name = _txt_fields[0]->getTexte();
            _level_name = _level_name + ".txt";
            auto* command = Game::CommandFactory::createLevel(_user_name, _level_name.c_str(), "", true);
            MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
            return MNAME::NOTHING;
        }
        else if (_buttons[15]->isClickOn()) {
            _level_name = DEFAULT_LEVEL;
            auto* command = Game::CommandFactory::createLevel(_user_name, _level_name.c_str(), "", true);
            MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
            return MNAME::NOTHING;
        }
        else
            setSettings();
    }
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            _txt_fields[0]->update(static_cast<char>(event.text.unicode));
        }
        _level_name = _txt_fields[0]->getTexte();
    }
    return MNAME::NOTHING;
}

void Menu::RoomMenuSFML::animation() noexcept{
   Menu::BaseMenuSFML::animation();
}

void Menu::RoomMenuSFML::startHiglight() noexcept {
    if (_friend_name[0]!='\0') {
        _buttons[0]->select();
    }
    _buttons[2]->select();
    _buttons[5]->select();
    _buttons[7]->select();
    _buttons[10]->select();
}

void Menu::RoomMenuSFML::newHilight(size_t first, size_t last, size_t who) noexcept {
    for(size_t i=first;i<=last;i++)
        _buttons[i]->unselect();
    _buttons[who]->select();
}

void Menu::RoomMenuSFML::setSettings() noexcept {
    if (_buttons[1]->isClickOn()) {
        newHilight(1, 3, 1);
        _difficulty=Game::GameSettings::Difficulty::Easy;
    }else if (_buttons[2]->isClickOn()) {
        newHilight(1, 3, 2);
        _difficulty=Game::GameSettings::Difficulty::Normal;
    }else if (_buttons[3]->isClickOn()) {
        newHilight(1, 3, 3);
        _difficulty=Game::GameSettings::Difficulty::Hard;
    }else if (_buttons[4]->isClickOn()) {
        newHilight(4, 6, 4);
        _dropping_rate=Game::GameSettings::DropRate::High;
    }else if (_buttons[5]->isClickOn()) {
        newHilight(4, 6, 5);
        _dropping_rate=Game::GameSettings::DropRate::Average;
    }else if (_buttons[6]->isClickOn()) {
        newHilight(4, 6, 6);
        _dropping_rate=Game::GameSettings::DropRate::None;
    }else if (_buttons[7]->isClickOn()) {
        newHilight(7, 8, 7);
        _friendly_fire=Game::GameSettings::FriendlyFire::No;
    }else if (_buttons[8]->isClickOn()) {
        newHilight(7, 8, 8);
        _friendly_fire=Game::GameSettings::FriendlyFire::Yes;
    }else if (_buttons[9]->isClickOn()) {
        newHilight(9, 11, 9);
        _lives=Game::GameSettings::HealthPoints::Few;
    }else if (_buttons[10]->isClickOn()){
        newHilight(9, 11, 10);
        _lives=Game::GameSettings::HealthPoints::Some;
    }else if (_buttons[11]->isClickOn()) {
        newHilight(9, 11, 11);
        _lives=Game::GameSettings::HealthPoints::Many;
    }
}

void Menu::RoomMenuSFML::setTextError(bool work) noexcept{
    if (work) {
        _level_name_correct = false;
        _textes[1]->setTexte("Level not found...");
    }else{
        setLevelPlayed(_level_name);
        _level_name_correct = true;
        _textes[1]->setTexte("Level found !");
    }
}