#include "EndMenuSFML.hpp"
#include "../RoomMenu.hpp"

Menu::EndMenuSFML::EndMenuSFML(int level_remain, int score_user, int score_friend) noexcept:
                        EndMenu(level_remain, score_user, score_friend) {
    initAll();
}

void Menu::EndMenuSFML::initAll() noexcept {
    if (_background==nullptr) {
        _background = new SFML::Background("sprites/background/background_1");
        _stars = new SFML::Stars("sprites/background/stars_");
    }
    _buttons.emplace_back(new SFML::Button(50.f, HEIGHT_WINDOW_SFML-175.f, 430.f, 85.f, "sprites/button/simple_grey", "ROOM MENU", 33, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML-480.f,  HEIGHT_WINDOW_SFML-175.f, 430.f, 85.f, "sprites/button/simple_grey", "LEADERBOARD", 33, COLORS_SFML::WHITE));
    if (isLevelACampaign()){
        // SI il s'agit d'un level de campagne on affiche le bouton en dehors de l'écran
        _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML+350.f,  170.f, 300.f, 80.f, "sprites/button/simple_grey", "", 30, COLORS_SFML::YELLOW));
    }else{
        _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML-350.f,  170.f, 300.f, 80.f, "sprites/button/simple_grey", "LIKE", 30, COLORS_SFML::YELLOW));
    }

    if (_friend_name[0] != '\0') {
        if (_score_user < _score_friend) {
            _textes.emplace_back(new SFML::Texte(_user_name, 885.f, 40.f, 33, COLORS_SFML::WHITE));
            _textes.emplace_back(new SFML::Texte("Score:"+std::to_string(_score_user), 835.f, 115.f, 33, COLORS_SFML::WHITE));
            _textes.emplace_back(new SFML::Texte(_friend_name, 80.f, 40.f, 33, COLORS_SFML::WHITE));
            _textes.emplace_back(new SFML::Texte("Score:"+std::to_string(_score_friend), 30.f, 115.f, 33, COLORS_SFML::WHITE));
        } else {
            _textes.emplace_back(new SFML::Texte(_user_name, 80.f, 40.f, 33, COLORS_SFML::WHITE));
            _textes.emplace_back(new SFML::Texte("Score:"+std::to_string(_score_user), 30.f, 115.f, 33, COLORS_SFML::WHITE));
            _textes.emplace_back(new SFML::Texte(_friend_name, 885.f, 40.f, 33, COLORS_SFML::WHITE));
            _textes.emplace_back(new SFML::Texte("Score:"+std::to_string(_score_friend), 835.f, 115.f, 33, COLORS_SFML::WHITE));
        }
    }else {
            _textes.emplace_back(new SFML::Texte(_user_name, 80.f, 40.f, 33, COLORS_SFML::WHITE));
            _textes.emplace_back(new SFML::Texte("Score:"+std::to_string(_score_user), 30.f, 115.f, 33, COLORS_SFML::WHITE));
    }
    if (_level_remain<=0){
        if (_level_remain==0) {
            _sprites.emplace_back(new SFML::SpriteSimple(-180, HEIGHT_WINDOW_SFML/2+45, 150, 100, "sprites/spaceship/Player/spaceship"));
            _sprites[0]->rotate(90.f);
            _sprites.emplace_back(new SFML::SpriteSimple(0, 0, WIDTH_WINDOW_SFML, HEIGHT_WINDOW_SFML, "sprites/titles/end/mario"));
        }
        else {
            _sprites.emplace_back(new SFML::SpriteSimple(75, 230, 1200.f, 400.f, "sprites/titles/end/win_commu_pixel"));
            _animation_end=false;
            _sprites.emplace_back(new SFML::SpriteSimple(WIDTH_WINDOW_SFML/2-500, HEIGHT_WINDOW_SFML/2-500, 1000.f, 1000.f, "sprites/animations/MassiveExplosion/explosion_", 25));
        }
    }
    else
        _sprites.emplace_back(new SFML::SpriteSimple(-150.f, 300.f, 120.f, 120.f*1.33f, "sprites/titles/end/among_us_red"));
    
    if (!isLevelACampaign()){
        auto* command = Game::CommandFactory::isliked(_user_name, RoomMenu::getLevelPlayed());
        MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
    }
}

MNAME Menu::EndMenuSFML::windowInputs(sf::Event &event) noexcept {
    if (event.type == sf::Event::MouseMoved) {
        for (auto & _button : _buttons)
            _button->isOn(event.mouseMove.x, event.mouseMove.y);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (_buttons[0]->isClickOn()) {
            quitSound();
            return MNAME::ROOM_MENU;
        }
        else if (_buttons[1]->isClickOn()) {
            quitSound();
            logoutPlayer2(getFriendName());
            return MNAME::LEADERBOARD_MENU;
        }else if (_buttons[2]->isClickOn()){
            Game::Command* command;
            if (_is_like){
                _buttons[2]->setTxt("DISLIKE");
                command = Game::CommandFactory::likeLevel(_user_name, RoomMenu::getLevelPlayed());
                _is_like = false;
            }else{
                _buttons[2]->setTxt("LIKE");
                command = Game::CommandFactory::likeLevel(_user_name, RoomMenu::getLevelPlayed());
                _is_like = true;
            }
            MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command); 
        }

    }
    return MNAME::NOTHING;
}

void Menu::EndMenuSFML::animation() noexcept {
    Menu::BaseMenuSFML::animation();
    if (_level_remain<=0) {
        winAnimation();
    }else
        looseAnimation();
}

void Menu::EndMenuSFML::looseAnimation() noexcept {
    if (_animation_timer<1500)
        _animation_timer++;
    if (!_animation_end) {
        _sprites[1]->animate(0.12f);
        if (_sprites[1]->isLastFrame())
            _animation_end=true;
    }
    if (_animation_timer<800) {
        _sprites[0]->move(5.f, 0);
        _sprites[0]->rotate(0.3f);
    }
    if (_animation_timer==160) {
        _sprites.emplace_back(new SFML::SpriteSimple(WIDTH_WINDOW_SFML/2-534.f/2, 370.f, 64.f*8.34f, 64.f, "sprites/titles/end/lose/lose_text_", 10));
        _animation_end=false;
    }
    if (_animation_timer==1000) {
        delete _sprites[0];
        _sprites.erase(_sprites.begin());
    }
    if (_animation_timer==250)
        _textes.emplace_back(new SFML::Texte(std::to_string(_level_remain)+" remaining levels", WIDTH_WINDOW_SFML/2.f-75.f, 495.f, 10, COLORS_SFML::WHITE));
    if (_animation_timer>250 && _animation_timer<300)
        _textes[_textes.size()-1]->zoom(1.022f);
}

void Menu::EndMenuSFML::winAnimation() noexcept {
    if (_level_remain==0 && _animation_timer<2000) {
        if (_animation_timer==80) {
            std::string tmp_thank = "THANK YOU ";
            tmp_thank += _user_name;
            if(_friend_name[0] != '\0') {
                tmp_thank += " and ";
                tmp_thank += _friend_name;
            }
            tmp_thank += "!";
            _textes.emplace_back(new SFML::Texte(tmp_thank, 335.f, 265.f, 36, COLORS_SFML::WHITE));
        }
        if (_animation_timer==160) {
            _textes.emplace_back(new SFML::Texte("YOUR QUEST IS OVER.", 300.f, 335.f, 36, COLORS_SFML::WHITE));
        }
        if (_animation_timer==240) {
            _textes.emplace_back(new SFML::Texte("YOUR CAN CREATE YOUR OWN LEVEL.", 150.f, 405.f, 36, COLORS_SFML::WHITE));
        }
        _animation_timer++;
        _sprites[0]->move(1.f, 0);
    } else {
        if (_animation_timer==25)
            _sprites.emplace_back(new SFML::SpriteSimple(WIDTH_WINDOW_SFML/2, HEIGHT_WINDOW_SFML/2+200, 100.f, 100.f, "sprites/animations/MassiveExplosion/explosion_", 25));
        else if (_animation_timer==3)
            _sprites.emplace_back(new SFML::SpriteSimple(25, 230, 120.f, 120.f, "sprites/animations/MassiveExplosion/explosion_", 25));
        else if (_animation_timer==7)
            _sprites.emplace_back(new SFML::SpriteSimple(500, 25, 110.f, 110.f, "sprites/animations/MassiveExplosion/explosion_", 25));
        else if (_animation_timer==13)
            _sprites.emplace_back(new SFML::SpriteSimple(1100, 400, 130.f, 130.f, "sprites/animations/MassiveExplosion/explosion_", 25));
        else if (_animation_timer==18)
            _sprites.emplace_back(new SFML::SpriteSimple(250, 600, 115.f, 115.f, "sprites/animations/MassiveExplosion/explosion_", 25));
        if (!_animation_end) {
            _sprites[1]->animate(0.4f);
            if (_sprites[1]->isLastFrame())
                _animation_end=true;
        }
        for (size_t i = 2; i < _sprites.size(); i++) {
            _sprites[i]->animate(0.5f);
        }
        _animation_timer++;
        
    }
}

void Menu::EndMenuSFML::setButtonLike(bool already_liked){
    if (already_liked){
        _is_like = false;
        _buttons[2]->setTxt("UNLIKE");
    } else {
        _is_like = true;
        _buttons[2]->setTxt("LIKE");
    }
}