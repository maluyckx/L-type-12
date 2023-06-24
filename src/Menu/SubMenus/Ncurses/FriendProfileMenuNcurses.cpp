#include "FriendProfileMenuNcurses.hpp"

const char* SPRITE_EARTH=
"               ____\n"
"          .-'\"\"p 8o\"\"`-.\n"
"       .-'8888P'Y.`Y[ ' `-.\n"
"     ,']88888b.J8oo_      '`.\n"
"   ,' ,88888888888[\"        Y`.\n"
"   /   8888888888P            Y8\\\n"
"  /    Y8888888P'             ]88\\\n"
":     `Y88'   P              `888:\n"
":       Y8.oP '- >            Y88:\n"
"|          `Yb  __             `'|\n"
":            `'d8888bo.          :\n"
":             d88888888ooo.      ;\n"
" \\            Y88888888888P     /\n"
"  \\            `Y88888888P     /\n"
"   `.            d88888P'    ,'\n"
"     `.          888PP'    ,'\n"
"       `-.      d8P'    ,-'\n"
"          `-.,,_'__,,.-'";
const char* SPRITE_SPACE_SHIP = 
"  .\n"
" .'.\n"
" |o|\n"
".'o'.\n"
"|.-.|\n"
"'   '\n"
" ( )\n"
"  )\n"
" ( )\n";

const char* SPRITE_MOON = 
"|       .--.          O   |\n"            
" | .   |    |            |\n"
"  \\    `.__.'    o   .  /\n"    
"   \\                   /\n"                   
"    `\\  o    ()      /'\n"         
"      `--___   ___--'\n"
"            ---";

Menu::FriendProfileMenuNcurses::FriendProfileMenuNcurses() noexcept {
    Menu::ProfileMenuNcurses::initAll();
    initAll();
}

void Menu::FriendProfileMenuNcurses::initAll() noexcept {
    _choice=1;
    _window->createObj(DECORATE_WINDOW, SPRITE_SPACE_SHIP, 100, SIZE::HEIGHT, 1);
    _window->createObj(DECORATE_WINDOW, SPRITE_EARTH, 85, 30, 1);
    _window->createObj(DECORATE_WINDOW, SPRITE_MOON, 90, 1, 1); 
    _window->createObj(MAIN_WINDOW, _friend_name, (SIZE::WIDTH/2-5) - 4, 5, 1);
    _window->createObj(MAIN_WINDOW, "", SIZE::WIDTH/2 - 10, 11, 1);
    _window->createButton("Level", 35, 13, 46, 5);
    _window->createButton(FRIEND_TXT[0], 35, 19, 46, 5);
    getFriendInfo();
}

void Menu::FriendProfileMenuNcurses::display() {
    Menu::ProfileMenuNcurses::display();
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::FriendProfileMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    switch (_window->input()) {
        case KEY::UP:
            if (_choice==0 && getFriendState()==Menu::FRIEND_STATE::ACCEPT)
                _choice = 3;
            else if (_choice==0 || _choice==3)
                _choice = 2;
            else if (_choice==2)
                _choice=1;
        break;
        case KEY::DOWN:
            if (_choice==2 && getFriendState()==Menu::FRIEND_STATE::ACCEPT)
                _choice = 3;
            else if (_choice==2 || _choice==3)
                _choice = 0;
            else if (_choice==1)
                _choice = 2;
        break;
        case KEY::LEFT:    
            _choice = 0;
        break;
        case KEY::RIGHT:
            if (_choice==0 && getFriendState()==Menu::FRIEND_STATE::ACCEPT)
                _choice = 2;
        break;
        case KEY::ENTER:
            if (_choice==0)
                return MNAME::SOCIAL_MENU;
            else if (_choice==1)
                return MNAME::FRIEND_LEVEL_MENU;
            else if (getFriendState() == Menu::FRIEND_STATE::FRIEND) 
                deleteFriend();
            else if (getFriendState() == Menu::FRIEND_STATE::NOT_FRIEND)
                sendFriendRequest();
            else if (_choice == 2 && getFriendState() == Menu::FRIEND_STATE::ACCEPT)
                acceptFriendRequest();
            else if (_choice == 3 && getFriendState() == Menu::FRIEND_STATE::ACCEPT)
                rejectFriendRequest();
        break;
        default:
            break;
    }
    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

void Menu::FriendProfileMenuNcurses::animation() noexcept {
    Menu::BaseMenuNcurses::animation();
    _ship_frame++;
    if (_ship_frame==4) {
        _window->move(_stars+2, _window->getPos(_stars+2, true), _window->getPos(_stars+2, false)-1);
        if (_window->getPos(_stars+2, false)==-4)
            _window->move(_stars+2, _window->getPos(_stars+2, true), SIZE::HEIGHT);
        _ship_frame=0;
    }
}

void Menu::FriendProfileMenuNcurses::sendFriendRequest() {
    FriendProfileMenu::sendFriendRequest();
    _window->changeObject(_window->getNbObject()-1, "Request sent", 1);
}

void Menu::FriendProfileMenuNcurses::deleteFriend() {
    FriendProfileMenu::deleteFriend();
    _window->changeButtons(1, FRIEND_TXT[Menu::FRIEND_STATE::NOT_FRIEND]);
}

void Menu::FriendProfileMenuNcurses::acceptFriendRequest() {
    FriendProfileMenu::acceptFriendRequest();
    _window->changeButtons(1, FRIEND_TXT[Menu::FRIEND_STATE::FRIEND]);
    _window->changeButtons(2, strdup(""));
}

void Menu::FriendProfileMenuNcurses::rejectFriendRequest() {
    FriendProfileMenu::rejectFriendRequest();
    _window->changeButtons(1, FRIEND_TXT[Menu::FRIEND_STATE::NOT_FRIEND]);
    _window->changeButtons(2, strdup(""));
}

void Menu::FriendProfileMenuNcurses::setFriendInfo(Menu::FRIEND_STATE state, int score) noexcept {
    setFriendState(state);
    _window->changeObject(_stars+1, "Score: "+std::to_string(score), 1);
    _window->changeButtons(2, FRIEND_TXT[getFriendState()]);
    if (getFriendState()==Menu::FRIEND_STATE::ACCEPT)
        _window->createButton("REJECT REQUEST", 35, 24, 46, 5);
}