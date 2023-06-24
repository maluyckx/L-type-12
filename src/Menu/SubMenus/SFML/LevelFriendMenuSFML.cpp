#include "LevelFriendMenuSFML.hpp"
#include "../RoomMenu.hpp"

#define NB_BTN 1

Menu::LevelFriendMenuSFML::LevelFriendMenuSFML() noexcept {
    initAll();
}

void Menu::LevelFriendMenuSFML::initAll() noexcept {
    _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));

    _sprites.emplace_back(new SFML::SpriteSimple(100.f, 10.f, 1100.f, 880.f, "sprites/background/rectangle/fond_simple"));
    _view_scroll = sf::View(sf::FloatRect(0, 0, 990.f, 700.f));
    _textes.emplace_back(new SFML::Texte(static_cast<std::string>(MenuInfo::getFriendName()) + "'s levels", 490.f, 50.f, 25, COLORS_SFML::WHITE));
    _view_scroll.setViewport(sf::FloatRect(static_cast<float>(0.18), static_cast<float>(0.14), static_cast<float>(0.72), static_cast<float>(0.76)));
    getLevels();
}

void Menu::LevelFriendMenuSFML::display() {  // Need to override for this fucking scrolling
    timer(30); //0.585996 0.133 0.366006 0.774003
    _window->setView(_window->getDefaultView());
    _background->draw(*_window);
    _stars->draw(*_window);
    for (size_t i = 0; i < _bg_sprites.size(); i++)
        _bg_sprites[i]->draw(*_window);
    for (size_t i = 0; i < _sprites.size(); i++)
        _sprites[i]->draw(*_window);
    for (size_t i = 0; i < _textes.size(); i++)
        _textes[i]->draw(*_window);
    for (size_t i = 0; i < _txt_fields.size(); i++)
        _txt_fields[i]->draw(*_window);   

    _window->setView(_view_scroll);
    for (size_t i = NB_BTN; i < _buttons.size(); i++)
        _buttons[i]->draw(*_window);  
    _window->setView(_window->getDefaultView());
    for (size_t i = 0; i < NB_BTN; i++)
        _buttons[i]->draw(*_window);
}

MNAME Menu::LevelFriendMenuSFML::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (size_t i = 0; i < NB_BTN; i++)
            _buttons[i]->isOn(event.mouseMove.x, event.mouseMove.y);

        _window->setView(_view_scroll);
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*_window);
        sf::Vector2f worldPos = _window->mapPixelToCoords(pixelPos);
        for (size_t i = NB_BTN; i < _buttons.size(); i++)
            _buttons[i]->isOn(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
        _window->setView(_window->getDefaultView());
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (_buttons[0]->isClickOn()){
            if (*MenuInfo::getUserName() != *MenuInfo::getFriendName()){
                return MNAME::PROFILE_FRIENDS;
            }else{
                MenuInfo::setFriendName("");
                return MNAME::LEVEL_MAIN_MENU;
            }
        }
        for (std::size_t i = NB_BTN; i < _buttons.size(); i++){
            if (_buttons[i]->isClickOn()){
                std::string level_name = _name_levels[i - 1];
                level_name += ".txt"; 
                RoomMenu::setLevelPlayed(level_name);
                MenuInfo::setFriendName("");
                return MNAME::ROOM_MENU;
            }
        }
    }
    if (event.type == sf::Event::MouseWheelMoved) {
      if (event.mouseWheel.delta==1) {
         if (_scrolling > 0)
            scrollBtn(10.f);
      }
      if (event.mouseWheel.delta==-1) {
         if (_buttons.size()>=NB_BTN+6&&
            static_cast<float>(_scrolling) < 50+105.f*static_cast<float>(_buttons.size()-(NB_BTN+7)))
            scrollBtn(-10.f);
      }
   }
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code){
            case sf::Keyboard::Up:
                if (_scrolling > 0)
                scrollBtn(5.f);
                break;
            case sf::Keyboard::Down:
                if (_buttons.size()>=NB_BTN+6 && 
                    static_cast<float>(_scrolling) < 50+105.f*static_cast<float>(_buttons.size()-(NB_BTN+7)))
                scrollBtn(-5.f);
                break;
            default:
                break;
        }
    }
    return MNAME::NOTHING;
}

void Menu::LevelFriendMenuSFML::scrollBtn(float scroll) noexcept {
   _scrolling -= scroll;
   for (size_t i = NB_BTN; i < _buttons.size(); i++) {
      _buttons[i]->update(0, scroll);
   }
}

void Menu::LevelFriendMenuSFML::animation() noexcept {
    Menu::BaseMenuSFML::animation();
}

void Menu::LevelFriendMenuSFML::getLevels() {
    auto* command = Game::CommandFactory::userLevelList(MenuInfo::getFriendName()); 
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}


void Menu::LevelFriendMenuSFML::createButtons() noexcept {
   for (size_t i = 0; i < _list_levels.size(); i++){
      _buttons.emplace_back(new SFML::Button(
            0, static_cast<float>(i)*105.f, 900.f, 100.f,
            "sprites/button/button_friends_", _list_levels[i], 30, COLORS_SFML::WHITE));
   }
   if (_buttons.size() == NB_BTN){
       _textes.emplace_back(new SFML::Texte(static_cast<std::string>(MenuInfo::getFriendName()) + " didn't create a level yet", 220.f, HEIGHT_WINDOW_SFML/2 - 300.f, 25, COLORS_SFML::YELLOW));
   }
}

 void Menu::LevelFriendMenuSFML::setLevelList(const char* levels, int* level_like, unsigned nb_levels) noexcept {
     for (size_t i = 0; i < nb_levels; i++) {
        char tmp_str[USERNAME_MAX_SIZE];
        int count_tmp = 0;
        for (size_t j = i*USERNAME_MAX_SIZE; j < USERNAME_MAX_SIZE*(i+1); j++) {
            tmp_str[count_tmp++] = levels[j];
        }
        std::string res = tmp_str;
        _name_levels.emplace_back(res);
        res += " : ";
        res += std::to_string(level_like[i]);
        res += " <3";
        _list_levels.emplace_back(res);
     }
     createButtons();
 }