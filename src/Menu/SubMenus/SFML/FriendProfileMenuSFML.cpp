#include "FriendProfileMenuSFML.hpp"

Menu::FriendProfileMenuSFML::FriendProfileMenuSFML() noexcept {
    initAll();
}

void Menu::FriendProfileMenuSFML::initAll() noexcept {
    _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));

    _textes.emplace_back(new SFML::Texte(_friend_name, 500.f, 50.f, 40, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("score", 400.f, 140.f, 40, COLORS_SFML::WHITE));
    getFriendInfo();
    setPlanet();
}

MNAME Menu::FriendProfileMenuSFML::windowInputs(sf::Event &event) noexcept {
    if(event.type == sf::Event::MouseMoved){
        for(auto & _button : _buttons)
            _button->isOn(event.mouseMove.x,event.mouseMove.y);
    }
    if(event.type == sf::Event::MouseButtonPressed){
        if (_buttons[0]->isClickOn())
            return MNAME::SOCIAL_MENU;
        else if(_buttons[2]->isClickOn()) {
            if (getFriendState() == Menu::FRIEND_STATE::FRIEND)
                deleteFriend();
            else if(getFriendState() == Menu::FRIEND_STATE::NOT_FRIEND)
                sendFriendRequest();
            else if(getFriendState() == Menu::FRIEND_STATE::ACCEPT)
                acceptFriendRequest();
        }
        else if (getFriendState() == Menu::FRIEND_STATE::ACCEPT && _buttons[3]->isClickOn())
            rejectFriendRequest();
        else if (_buttons[1]->isClickOn()){
            return MNAME::FRIEND_LEVEL_MENU; 
        }
    }
   return MNAME::NOTHING;
}

void Menu::FriendProfileMenuSFML::animation() noexcept {
   Menu::BaseMenuSFML::animation();
    _sprites[0]->animate(0.2f);
}

void Menu::FriendProfileMenuSFML::setPlanet() noexcept {
    int _planets_choice = static_cast<int>(_friend_name[0])%6;
    float pos_y=510;
    size_t frame_number=200;
    float size_wh = 325.f;
    if (getFriendState()==Menu::FRIEND_STATE::ACCEPT)
        pos_y=540;
    if (_planets_choice==0){
        frame_number=120;size_wh=500.f;pos_y = 590;}
    else if (_planets_choice==4) {
        frame_number=150;size_wh=600.f;pos_y = 655;}
   _sprites.emplace_back(new SFML::SpriteSimple(WIDTH_WINDOW_SFML/2-size_wh/2, pos_y-(size_wh-350),
                      size_wh, size_wh, "sprites/background/planets/planet_"+std::to_string(_planets_choice), frame_number, true));
}

void Menu::FriendProfileMenuSFML::sendFriendRequest() {
    FriendProfileMenu::sendFriendRequest();
    _buttons[2]->setTxt("REQUEST SENT");
}

void Menu::FriendProfileMenuSFML::deleteFriend() {
    FriendProfileMenu::deleteFriend();
    _buttons[2]->setTxt(FRIEND_TXT[Menu::FRIEND_STATE::NOT_FRIEND]);
}

void Menu::FriendProfileMenuSFML::acceptFriendRequest() {
    FriendProfileMenu::acceptFriendRequest();
    _buttons[2]->setTxt(FRIEND_TXT[Menu::FRIEND_STATE::FRIEND]);
    delete _buttons[3];
    _buttons.pop_back();
}

void Menu::FriendProfileMenuSFML::rejectFriendRequest() {
    FriendProfileMenu::rejectFriendRequest();
    _buttons[2]->setTxt(FRIEND_TXT[Menu::FRIEND_STATE::NOT_FRIEND]);
    delete _buttons[3];
    _buttons.pop_back();
}

void Menu::FriendProfileMenuSFML::setFriendInfo(Menu::FRIEND_STATE state, int score) noexcept {
    setFriendState(state);
    _textes[1]->setTexte("Score: "+std::to_string(score));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 300.f, 225.f, 600.f, 92.f,"sprites/button/btn_blue_", "Levels", 50, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 300.f, 330.f, 600.f, 92.f, "sprites/button/btn_blue_", FRIEND_TXT[getFriendState()], 40, COLORS_SFML::WHITE));
    if (getFriendState()==Menu::FRIEND_STATE::ACCEPT){
        _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML/2 - 300.f, 435.f, 600.f, 92.f, "sprites/button/btn_blue_", "REJECT", 40, COLORS_SFML::WHITE));}
    
}
