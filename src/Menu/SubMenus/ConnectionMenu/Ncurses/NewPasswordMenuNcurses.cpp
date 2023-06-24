#include "NewPasswordMenuNcurses.hpp"


Menu::NewPasswordMenuNcurses::NewPasswordMenuNcurses() noexcept {
    initAll();
}

void Menu::NewPasswordMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    _choice=2;
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    _window->createButton("DONE", 57, 24, 13, 3);
    _window->createButton("BACK", 3, 35, 10, 5);
    _window->createTxtInput(30, 45, 9, 40, 3, true);
    _window->createTxtInput(30, 45, 18, 40, 3, true);
    _window->createObj(MAIN_WINDOW, "NEW PASSWORD", 59, 7, 1);
    _window->createObj(MAIN_WINDOW, "RE-TYPE PASSWORD", 57, 16, 1);
    _window->createObj(MAIN_WINDOW, "", 46, 22, 1);
}

void Menu::NewPasswordMenuNcurses::display(){
    timer(50);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawTxtInput();
    _window->drawBox(MAIN_WINDOW, 1);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::NewPasswordMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    switch (_window->writeText(_choice-_window->getNbButtons())) {
        case KEY::UP:
            if (_choice==0)
                _choice = 3;
            else if (_choice==3)
                _choice = 2;
            else if (_choice==1)
                _choice = 0;
        break;
        case KEY::DOWN:
            if (_choice==0)
                _choice = 1;
            else if (_choice==2)
                _choice = 3;
            else if (_choice==3)
                _choice=0;
        break;
        case KEY::LEFT:
            _choice = 1;
        break;
        case KEY::RIGHT:
            _choice = 0;
        break;
        case KEY::TAB:
            _choice++;
            if (_choice==_window->getNbButtons()+2)
                _choice=0;
        break;
        case KEY::ENTER:
            if (_choice == 0)
                return setPassword(); 
            else if (_choice == 1)
                return MNAME::LOGIN_MENU;
        break;
        default:
            break;
    }
    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

bool Menu::NewPasswordMenuNcurses::testPassword() noexcept {
    if ((strcmp(_window->getText(0), _window->getText(1)) == 0))
        return true;
    _window->changeObject(_window->getNbObject()-1, "The two passwords are different !", 1);
    return false;
}

MNAME Menu::NewPasswordMenuNcurses::setPassword() noexcept {
     if (testValues() && testPassword()){
        auto* command = Game::CommandFactory::authRestore(
            Game::Command::AuthRestore::State::NewPassword
            ,_user_name,
            _window->getText(0),
            strdup(""),
            strdup(""));
        _client->getConnection()->getSendingCommandQueue()->push(command);
        return MNAME::LOGIN_MENU;
     }  
     return MNAME::NOTHING;
 }

 void Menu::NewPasswordMenuNcurses::setTextError() noexcept {
    _window->changeObject(_window->getNbObject()-1, "Bug or something", 1);
 }