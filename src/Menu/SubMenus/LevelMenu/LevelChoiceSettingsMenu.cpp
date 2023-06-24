#include "LevelChoiceSettingsMenu.hpp"

// HHHHHHHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA PTN D'ATTRIBUTS STATIQUES DE MERDE
int   Menu::LevelChoiceSettingsMenu::_timer_level;
char  Menu::LevelChoiceSettingsMenu::_name_level[LEVEL_NAME_MAX_SIZE];

Menu::LevelChoiceSettingsMenu::LevelChoiceSettingsMenu() noexcept {
    initAll();
}

void Menu::LevelChoiceSettingsMenu::initAll() noexcept {
    // Back - Create buttons
    _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML - 400.f,  HEIGHT_WINDOW_SFML-100.f, 350.f, 60.f, "sprites/button/btn_blue_", "Create", 50, COLORS_SFML::WHITE));
    // Time buttons
    _buttons.emplace_back(new SFML::Button(540, 110, 250, 80, "sprites/button/btn_select_", "SHORT",  40,  COLORS_SFML::GREEN, true));
    _buttons.emplace_back(new SFML::Button(490, 220, 350, 80, "sprites/button/btn_select_", "AVERAGE",  40, COLORS_SFML::YELLOW, true));
    _buttons.emplace_back(new SFML::Button(540, 330, 250, 80, "sprites/button/btn_select_", "LONG",  40,  COLORS_SFML::RED, true));
    // Error message field
    _textes.emplace_back(new SFML::Texte("", 100, 620, 23.f, COLORS_SFML::RED));
    // Labels
    _textes.emplace_back(new SFML::Texte("Level duration", 500, 50, 23.f, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Level name", 550, 450, 23.f, COLORS_SFML::WHITE));
    // Input name level
    _txt_fields.emplace_back(new SFML::TexteField(500, 500, 340, 80, COLORS_SFML::WHITE, 23.f, COLORS_SFML::WHITE, 14)); // 4 Is for .txt
}

MNAME Menu::LevelChoiceSettingsMenu::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (size_t i = 0; i < _buttons.size(); i++)
            _buttons[i]->isOn(event.mouseMove.x, event.mouseMove.y);
        for (size_t i = 0; i < _txt_fields.size(); i++)
            _txt_fields[i]->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (_buttons[0]->isClickOn()){
            return MNAME::LEVEL_MAIN_MENU;
        }else if (_buttons[1]->isClickOn()){
            if (testValues()){
                strcpy(_name_level, _txt_fields[0]->getTexte());
                strcat(_name_level, ".txt");
                _timer_level = _choosen_time;
                auto* command = Game::CommandFactory::createLevel(_user_name, _name_level, "", true);
                _client->getConnection()->getSendingCommandQueue()->push(command);
            }
            return MNAME::NOTHING;
        }else if (_buttons[2]->isClickOn()){
            _choosen_time = 2;
            _buttons[2]->select();
            _buttons[3]->unselect();
            _buttons[4]->unselect();
            return MNAME::NOTHING;
        }else if (_buttons[3]->isClickOn()){
            _buttons[2]->unselect();
            _buttons[3]->select();
            _buttons[4]->unselect();
            _choosen_time = 5;
            return MNAME::NOTHING;
        }else if (_buttons[4]->isClickOn()){
            _buttons[2]->unselect();
            _buttons[3]->unselect();
            _buttons[4]->select();
            _choosen_time = 10;
            return MNAME::NOTHING;
        }
        
        for (size_t i = 0; i < _txt_fields.size(); ++i)
            _txt_fields[i]->isCLickOn();
    }
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            for (size_t i = 0; i < _txt_fields.size(); ++i) {
                _txt_fields[i]->update(static_cast<char>(event.text.unicode));
            }
        }
    }
    return MNAME::NOTHING;
}

void Menu::LevelChoiceSettingsMenu::animation() noexcept {
    Menu::BaseMenuSFML::animation();
}

void Menu::LevelChoiceSettingsMenu::setTextError() noexcept{
   _textes[0]->setTexte("Name of Level already taken");
}

bool Menu::LevelChoiceSettingsMenu::testValues() noexcept {
    if (_choosen_time == -1){
        _textes[0]->setTexte("Please select a time.");
        return false;
    }
    for (size_t i = 0; i < _txt_fields.size(); i++) {
        std::string tmp_lenght = _txt_fields[i]->getTexte();
        if (tmp_lenght.length() < 3) {
            _textes[0]->setTexte("Level name must be filled. (min. 3 char)");
            return false;
        }
    }
    return true;
}