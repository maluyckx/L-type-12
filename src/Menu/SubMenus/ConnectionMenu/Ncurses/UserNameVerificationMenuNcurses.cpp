#include "UserNameVerificationMenuNcurses.hpp"


Menu::UserNameVerificationMenuNcurses::UserNameVerificationMenuNcurses() noexcept {
    initAll();
}

void Menu::UserNameVerificationMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    _choice=2;
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    _window->createButton("DONE", 59, 20, 13, 3);
    _window->createButton("BACK", 3, 35, 10, 5);
    _window->createTxtInput(30, 45, 15, 40, 3);
    _window->createObj(MAIN_WINDOW, "USERNAME", 61, 12, 1);
    _window->createObj(MAIN_WINDOW, "", 46, 18, 1);
}

void Menu::UserNameVerificationMenuNcurses::display(){
    timer(50);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawTxtInput();
    _window->drawBox(MAIN_WINDOW, 0);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::UserNameVerificationMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    switch (_window->writeText(_choice-_window->getNbButtons())) {
        case KEY::UP:
            if (_choice == 1)
                _choice = 0;
            else if (_choice == 0)
                _choice = 2;
        break;
        case KEY::DOWN:
            if (_choice == 0)
                _choice = 1;
            else if (_choice==_window->getNbButtons())
                _choice = 0;
        break;
        case KEY::LEFT:
            _choice = 1;
        break;
        case KEY::RIGHT:
            _choice = 0;
        break;
        case KEY::TAB:
            _choice++;
            if (_choice==_window->getNbButtons()+1)
                _choice=0;
        break;
        case KEY::ENTER:
            if (_choice == 0)
                return testUser(); 
            else if (_choice == 1)
                return MNAME::LOGIN_MENU;
        break;
        default:
            break;
    }
     _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

 MNAME Menu::UserNameVerificationMenuNcurses::testUser() noexcept {
     if (testValues()){
        auto* command = Game::CommandFactory::authRestore(
            Game::Command::AuthRestore::State::RequestSecretQuestion,
            _window->getText(0),
         strdup(""),
         strdup(""),
         strdup(""));
        _client->getConnection()->getSendingCommandQueue()->push(command);
        strcpy(_user_name, _window->getText(0));
     }
     return MNAME::NOTHING;
 }

 void Menu::UserNameVerificationMenuNcurses::setTextError() noexcept {
    _window->changeObject(_window->getNbObject()-1, "Bad username !", 1);
 }