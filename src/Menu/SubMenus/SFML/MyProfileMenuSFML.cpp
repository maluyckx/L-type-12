#include "MyProfileMenuSFML.hpp"

const size_t BTN_PROFILE = 2;

Menu::MyProfileMenuSFML::MyProfileMenuSFML() noexcept {
    initAll();
}

void Menu::MyProfileMenuSFML::initAll() noexcept {
    getUser();
    _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(160.f,  530.f, 400.f, 80.f, "sprites/button/simple_grey", "Change", 45, COLORS_SFML::WHITE));
    _txt_fields.emplace_back(new SFML::TexteField(100.f, 230.f, 550.f, 70.f, COLORS_SFML::WHITE, 25, COLORS_SFML::WHITE, 19, true));
   _sprites.emplace_back(new SFML::SpriteSimple(WIDTH_WINDOW_SFML-580.f, 10.f, 560.f, 880.f, "sprites/background/rectangle/fond_simple"));
    creatTexte();
   _view_scroll = sf::View(sf::FloatRect(0, 0, 500.f, 700.f));
   _view_scroll.setViewport(sf::FloatRect(0.606996f, 0.131f, 0.338006f, 0.772003f));
}

void Menu::MyProfileMenuSFML::display() {  // Need to override for this fucking scrolling
    timer(30);
    _window->setView(_window->getDefaultView());
   _background->draw(*_window);
   _stars->draw(*_window);
   for (auto & _bg_sprite : _bg_sprites)
      _bg_sprite->draw(*_window);
   for (auto & _sprite : _sprites)
      _sprite->draw(*_window);
   for (auto & _texte : _textes)
      _texte->draw(*_window);
   for (auto & _txt_field : _txt_fields)
      _txt_field->draw(*_window);

   for (size_t i = 0; i < BTN_PROFILE; i++)
      _buttons[i]->draw(*_window);
   _window->setView(_view_scroll);
   for (size_t i = BTN_PROFILE; i < _buttons.size(); i++)
      _buttons[i]->draw(*_window);
}

MNAME Menu::MyProfileMenuSFML::windowInputs(sf::Event &event) noexcept {
    if(event.type == sf::Event::MouseMoved){
        for(size_t i = 0; i < BTN_PROFILE; i++)
            _buttons[i]->isOn(event.mouseMove.x,event.mouseMove.y);
        _txt_fields[0]->isOn(event.mouseMove.x, event.mouseMove.y);

        _window->setView(_view_scroll);
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*_window);
        sf::Vector2f worldPos = _window->mapPixelToCoords(pixelPos);
        for (size_t i = BTN_PROFILE; i < _buttons.size(); i++)
            _buttons[i]->isOn(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
        _window->setView(_window->getDefaultView());
    }
    if (event.type == sf::Event::MouseWheelMoved) {
        if (event.mouseWheel.delta==1) {
            if (_scrolling > 0)
                scrollBtn(10.f);
        }
        if (event.mouseWheel.delta==-1) {
            if (_buttons.size()>=BTN_PROFILE+6&&
                static_cast<float>(_scrolling) < 50+105.f*static_cast<float>(_buttons.size()-(BTN_PROFILE+7)))
                scrollBtn(-10.f);
        }
    }
    if(event.type == sf::Event::MouseButtonPressed){
        _txt_fields[0]->isCLickOn();
        if (_buttons[0]->isClickOn())
            return MNAME::SOCIAL_MENU;
        else if(_buttons[1]->isClickOn())
            changeInfo();
        else
            return goTo();
    }
    if (event.type==sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter)
            changeInfo();
        if (event.key.code == sf::Keyboard::Up) {
            if (_scrolling > 0)
                scrollBtn(5.f);
      }
      if (event.key.code==sf::Keyboard::Down) {
        if (_buttons.size()>=BTN_PROFILE+6&&
            static_cast<float>(_scrolling) < 50+105.f*static_cast<float>(_buttons.size()-(BTN_PROFILE+7)))
            scrollBtn(-5.f);
      }
    } else if (event.type==sf::Event::TextEntered) {
        if (event.text.unicode < 128)
            _txt_fields[0]->update(static_cast<char>(event.text.unicode));
    }
   return MNAME::NOTHING;
}

void Menu::MyProfileMenuSFML::animation() noexcept{
   Menu::BaseMenuSFML::animation();
   _sprites[1]->animate(0.2f);
}

void Menu::MyProfileMenuSFML::setPlanet() noexcept {
    int _planets_choice = static_cast<int>(_user_name[0])%6;
    size_t frame_number=200;
    float size_wh = 200.f;
    float planet_4_y=0;
    if (_planets_choice==0){
        frame_number=120;size_wh=300.f;}
    else if (_planets_choice==4) {
        frame_number=150;size_wh=350.f;planet_4_y=50;}
   _sprites.emplace_back(new SFML::SpriteSimple(WIDTH_WINDOW_SFML/2-(size_wh/2+80.f), planet_4_y+650.f-(size_wh-200),
                      size_wh, size_wh, "sprites/background/planets/planet_"+std::to_string(_planets_choice), frame_number, true));
}

void Menu::MyProfileMenuSFML::scrollBtn(float scroll) noexcept {
   _scrolling -= scroll;
   for (size_t i = BTN_PROFILE; i < _buttons.size(); i++) {
      _buttons[i]->update(0, scroll);
   }
}

MNAME Menu::MyProfileMenuSFML::goTo() noexcept {
    for (size_t i = BTN_PROFILE; i < _buttons.size(); i++){
        if (_buttons[i]->isClickOn()) {
            setFriendName(_friends_waiting[i-BTN_PROFILE]);
            return MNAME::PROFILE_FRIENDS;
        }
   }
    return MNAME::NOTHING;
}

void Menu::MyProfileMenuSFML::setUser(int score, char* question, char* answer, const char* pending_list) {
    _textes[0]->setTexte("Score: "+std::to_string(score));
    _textes[1]->setTexte(question);
    _textes[2]->setTexte(answer);
    size_t i = 0;
    while (pending_list[i*32] != '\0') {
        char tmp_str[32];
        int count_tmp=0;
        for (size_t j = i*32; j < 32*(i+1); j++) {
            tmp_str[count_tmp++] = pending_list[j];
        }
        _friends_waiting.emplace_back(tmp_str);
        i++;
    }
    createButtons();
    setPlanet();
}

void Menu::MyProfileMenuSFML::creatTexte() noexcept {
    _textes.emplace_back(new SFML::Texte("-score-", 220.f, 105.f, 30, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("-Question-", 130.f, 365.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("-Answer-", 130.f, 465.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("", 90.f, 505.f, 20, COLORS_SFML::YELLOW));
    _textes.emplace_back(new SFML::Texte(_user_name, 250.f, 35.f, 30, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Password", 90.f, 170.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Secret question :", 90.f, 325.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Secret answer :", 90.f, 425.f, 25, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("Request", 930.f, 50.f, 25, COLORS_SFML::WHITE));
}

void Menu::MyProfileMenuSFML::createButtons() noexcept {
   for (size_t i = 0; i < _friends_waiting.size(); i++){
      _buttons.emplace_back(new SFML::Button(
            0, static_cast<float>(i)*105.f, 500.f, 100.f,
            "sprites/button/button_friends_", _friends_waiting[i], 30, COLORS_SFML::WHITE));
   }
}

void Menu::MyProfileMenuSFML::changeInfo() {
    std::string tmp_lenght=_txt_fields[0]->getTexte();
    if (tmp_lenght.length()>=3) {
        Menu::MyProfileMenu::changeInfo(_txt_fields[0]->getTexte());
        _textes[3]->setTexte("Password update !");
    }
    else
        _textes[3]->setTexte("Password need at least 3 char");
}
