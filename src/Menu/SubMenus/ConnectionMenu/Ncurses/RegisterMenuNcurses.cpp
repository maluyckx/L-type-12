#include "RegisterMenuNcurses.hpp"

Menu::RegisterMenuNcurses::RegisterMenuNcurses() noexcept {
    initAll();
}

void Menu::RegisterMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    _choice=2;
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    _window->createButton("REGISTER", 57, 28, 15, 3);
    _window->createButton("BACK", 3, 35, 10, 5);
    _window->createTxtInput(32, 45, 4, 40, 3);
    _window->createTxtInput(32, 45, 9, 40, 3, true);
    _window->createTxtInput(60, 45, 14, 64, 3);
    _window->createTxtInput(32, 45, 19, 40, 3);
    _window->createObj(MAIN_WINDOW, "USERNAME:", 34, 5, 1);
    _window->createObj(MAIN_WINDOW, "PASSWORD:", 34, 10, 1);
    _window->createObj(MAIN_WINDOW, "SECRET QUESTION:", 27, 15, 1);
    _window->createObj(MAIN_WINDOW, "ANSWER OF THE QUESTION:", 20, 20, 1);
    _window->createObj(MAIN_WINDOW, "", 46, 23, 1);
}

void Menu::RegisterMenuNcurses::display(){
    timer(50); 
    _window->drawObjects();
    _window->drawButtons();
    _window->drawTxtInput();   
    _window->drawBox(MAIN_WINDOW, 1);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::RegisterMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    switch (_window->writeText(_choice-_window->getNbButtons())) {
        case KEY::UP:
            if (_choice == 1)
                _choice = 0;
            else if (_choice==0)
                _choice = 5;            
            else if (_choice > 2)
                _choice--;
        break;
        case KEY::DOWN:
            if (_choice == 0)
                _choice = 1;
            else if (_choice >= 2 && _choice < 5)
                _choice++;
            else if (_choice==5)
                _choice=0;
            break;
        break;
        case KEY::LEFT:
            _choice = 1;
        break;
        case KEY::RIGHT:
            _choice = 0;
        break;
        case KEY::TAB:
            _choice++;
            if (_choice==_window->getNbButtons()+4)
                _choice=0;
        break;
        case KEY::ENTER:
            if (_choice == 0)
                createAccount(); 
            else if (_choice == 1)
                return MNAME::CONNECTION_MENU;
        break;
        default:
            break;
    }
     _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

void Menu::RegisterMenuNcurses::createAccount() {
    if (testValues()){
        auto* command = Game::CommandFactory::createAccount(
            _window->getText(0),
            _window->getText(1),
            _window->getText(2),
            _window->getText(3));
        _client->getConnection()->getSendingCommandQueue()->push(command);
    if (_is_user)
        strcpy(_user_name, _window->getText(0));
    else
        setFriendName(_window->getText(0));
    }
}

 void Menu::RegisterMenuNcurses::setTextError() noexcept {
    _window->changeObject(_window->getNbObject()-1, "Bad username !", 1);
 }