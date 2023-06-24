#include "ConnectionMenuNcurses.hpp"
#include "ConnectionInfoNcurses.hpp"
#include "../../../../Utils/Constants.hpp"

const std::string NAME_BTN[2] = {"LOGIN", "REGISTER"}; 
const char* SPRITE_DESERT = 
"                     _    +        ______                    '                   '+                 ___      +         ,   \n"
"       *     (      /|\\      _   _|      \\___       +    '              *                     _____/   \\                \n"
" +             /\\  ||||| .  | | |   | |      |            '                        '        _/          \\____    *       \n"
"________________||||_|||||____| |_|_____________\\__________________________________________/                 \\____________\n"
"            . |||| |||||  /\\   _____      _____  .   .       .             .       .                            ,          \n"
"             . \\|`-'|||| ||||    __________            .                                          ,                        \n"
"                \\__ |||| ||||      .          .     .     .        -            .   .                             .        \n"
"             __    ||||`-'|||  .       .    __________                                                 _____                \n" 
"            .    . |||| ___/  ___________             .            ,         .          -  ,                           ,    \n"
"           _   ___|||||__  _           .          _                         ___                             ---             \n"
"                 _ `---'    .   .    .   _   .   .    .                                    -       -                        \n"
"  .         _  ^      .  -    .    -    .       -    .    .  .      -   .     .    -   -                     -      -       \n"
"     _                                                                  _   __            __                   ___          \n";


Menu::ConnectionMenuNcurses::ConnectionMenuNcurses(Game::Client::GameClient* client) noexcept {
    MenuInfo::setClient(client);
    initAll();
}

void Menu::ConnectionMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    setFriendName("\0");
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    for (size_t i = 0; i < sizeof(NAME_BTN)/sizeof(NAME_BTN[0]); i++) {
        _window->createButton(NAME_BTN[i], 40,static_cast<int>(6+10*i), 46, 5);
    }
    if (_client != nullptr && _client->getSound() != nullptr) {
        if (_client->getSound()->isMute())
            _window->createButton("MUTE", 45, 23, 36, 3); 
        else
            _window->createButton("SOUND", 45, 23, 36, 3); 
    }else {
        _window->createButton("SOUND", 45, 23, 36, 3); 
    }
    _window->createButton("QUIT", 3, 35, 10, 5); 
}

void Menu::ConnectionMenuNcurses::display(){
    timer(50);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawBox(MAIN_WINDOW, 0);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::ConnectionMenuNcurses::inputs() {
     _window->unsetHiglight(_choice);
    switch (_window->input()) {
        case KEY::UP:
            if (_choice > 0)
                _choice--;
        break;
        case KEY::DOWN:
            if (_choice < 3) 
                _choice++;
        break;
        case KEY::LEFT:
            _choice = 3;
        break;
        case KEY::RIGHT:
            _choice = 0;
        break;
        case KEY::ENTER:
            if (_choice == 0)
                return MNAME::LOGIN_MENU; 
            else if (_choice == 1)
                return MNAME::REGISTER_MENU;
            else if (_choice == 2) {
                _client->getSound()->toggleMute();
                if (_client->getSound()->isMute())
                    _window->changeButtons(2, strdup("MUTE"));
                else
                    _window->changeButtons(2, strdup("SOUND"));
            }
            else if (_choice == 3) {
                if (Menu::ConnectionInfo::_is_user)
                    return MNAME::EXIT_MENU;
                return MNAME::ROOM_MENU;
            }
        break;
        default:
            break;
    }
    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

void Menu::ConnectionMenuNcurses::decorate() noexcept {
    Menu::BaseMenuNcurses::decorate();
    _window->createObj(DECORATE_WINDOW,SPRITE_DESERT, 2, 28, 1);
    _window->createObj(DECORATE_WINDOW, STYLE_PLAYERSHIP, -5, 2, 1);
    _window->createObj(DECORATE_WINDOW, STYLE_PLAYERSHIP, SIZE::WIDTH, 13, 1);
}

void Menu::ConnectionMenuNcurses::animation() noexcept {
    Menu::BaseMenuNcurses::animation();
    _ship_speed++;
    if (_ship_speed==4) {
        _window->move(_window->getNbObject()-2,
                      _window->getPos(_window->getNbObject()-2, true)+1,
                      _window->getPos(_window->getNbObject()-2, false));
        _window->move(_window->getNbObject()-1,
                      _window->getPos(_window->getNbObject()-1, true)-1,
                      _window->getPos(_window->getNbObject()-1, false));
        if (_window->getPos(_window->getNbObject()-2, true)==SIZE::WIDTH)
            _window->move(_window->getNbObject()-2,-5,5);
        if (_window->getPos(_window->getNbObject()-1, true)==-5)
            _window->move(_window->getNbObject()-1,SIZE::WIDTH, 15);
        _ship_speed=0;
    }
}