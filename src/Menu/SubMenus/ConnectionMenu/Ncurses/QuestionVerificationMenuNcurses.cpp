#include "QuestionVerificationMenuNcurses.hpp"


Menu::QuestionVerificationMenuNcurses::QuestionVerificationMenuNcurses(char* question_str) noexcept {
    strcpy(_question, question_str);
    initAll();
}

void Menu::QuestionVerificationMenuNcurses::initAll() noexcept {
     _window = new Menu::MenuMaker();
     _choice=2;
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    _window->createButton("DONE", 59, 20, 13, 3);
    _window->createButton("BACK", 3, 35, 10, 5);
    _window->createTxtInput(30, 45, 15, 40, 3);
    _window->createObj(MAIN_WINDOW, "Answer your secret question", 49, 10, 1);
    _window->createObj(MAIN_WINDOW, _question, static_cast<int>(WIDTH/2 - strlen(_question)/2), 12, 1);
    _window->createObj(MAIN_WINDOW, "", 46, 18, 1);
}

void Menu::QuestionVerificationMenuNcurses::display(){
    timer(50);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawTxtInput();
    _window->drawBox(MAIN_WINDOW, 1);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::QuestionVerificationMenuNcurses::inputs() {
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
                return testQuestion(); 
            else if (_choice == 1)
                return MNAME::LOGIN_MENU;
        break;
        default:
            break;
    }
     _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

MNAME Menu::QuestionVerificationMenuNcurses::testQuestion() noexcept {
     if (testValues()){
        auto* command = Game::CommandFactory::authRestore(
            Game::Command::AuthRestore::State::CheckSecretAnswer,
            _user_name,
            strdup(""),
            _question,
            _window->getText(0));
        _client->getConnection()->getSendingCommandQueue()->push(command);
     }
     return MNAME::NOTHING;
 }

 void Menu::QuestionVerificationMenuNcurses::setTextError() noexcept {
    _window->changeObject(_window->getNbObject()-1, "Bad response!", 1);
 }