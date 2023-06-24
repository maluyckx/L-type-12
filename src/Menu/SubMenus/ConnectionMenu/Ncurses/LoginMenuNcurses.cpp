#include "LoginMenuNcurses.hpp"


Menu::LoginMenuNcurses::LoginMenuNcurses() noexcept {
    initAll();
}

void Menu::LoginMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    _choice=3;
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    _window->createButton("BACK", 3, 35, 10, 5);
    _window->createButton("LOGIN", 55, 23, 15, 3);
    _window->createButton("RESSET PASSWORD", 98, 35, 25, 5);
    _window->createTxtInput(30, 45, 4, 40, 3);
    _window->createTxtInput(30, 45, 14, 40, 3, true);
    _window->createObj(MAIN_WINDOW, "USERNAME:", 34, 5, 1);
    _window->createObj(MAIN_WINDOW, "PASSWORD:", 34, 15, 1);
    _window->createObj(MAIN_WINDOW, "", 46, 17, 1);
}

void Menu::LoginMenuNcurses::display(){
    timer(50);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawTxtInput();
    _window->drawBox(MAIN_WINDOW, 0);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::LoginMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    switch (_window->writeText(_choice-_window->getNbButtons())) {
        case KEY::UP:
            if (_choice==0)
                _choice = 1;
            else if (_choice==1)
                _choice = 4;
            else if (_choice==4)
                _choice=3;
            else if (_choice==2)
                _choice = 1;
        break;
        case KEY::DOWN:
            if (_choice==1)
                _choice = 0;
            else if (_choice==3)
                _choice=4;
            else if (_choice==4)
                _choice=1;
        break;
        case KEY::LEFT:
            _choice = 0;
        break;
        case KEY::RIGHT:
            _choice = 2;
        break;
        case KEY::ENTER:
            if (_choice == 1)
                login();
            else if (_choice == 2)
                return MNAME::USER_NAME_VERIFICATION_MENU; 
            else if (_choice == 0)
                return MNAME::CONNECTION_MENU;
        break;
        case KEY::TAB:
            _choice++;
            if (_choice==_window->getNbButtons()+2)
                _choice=1;
        break;
        default:
            break;
    }
    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}


 void Menu::LoginMenuNcurses::login() {
     if (testValues()){
        if (_is_user) {
            auto* command = Game::CommandFactory::authLogin(
                    _window->getText(0),
                    _window->getText(1));
            _client->getConnection()->getSendingCommandQueue()->push(command);
            strcpy(_user_name, _window->getText(0));
        }else if (strcmp(_window->getText(0), _user_name) != 0) {
            auto* command = Game::CommandFactory::authLogin(
                    _window->getText(0),
                    _window->getText(1));
            _client->getConnection()->getSendingCommandQueue()->push(command);
            setFriendName(_window->getText(0));
        } else {
            _window->changeObject(_window->getNbObject()-1, "You can't play with you !", 1);
        }

     }
 }
 
 void Menu::LoginMenuNcurses::setTextError() noexcept{
    _window->changeObject(_window->getNbObject()-1, "Bad username or password !", 1);
 }