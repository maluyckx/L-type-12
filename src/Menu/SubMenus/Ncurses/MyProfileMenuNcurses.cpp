#include "MyProfileMenuNcurses.hpp"

const int DISPLAYED_BUTTON = 7;
const int STATIC_BUTTON = 3;

Menu::MyProfileMenuNcurses::MyProfileMenuNcurses() noexcept {
    Menu::ProfileMenuNcurses::initAll();
    initAll();
}

void Menu::MyProfileMenuNcurses::initAll() noexcept {
    getUser();
    _window->createButton("CHANGE", SIZE::WIDTH/2-20, 23, 35, 5);
    _window->createButton("My levels", SIZE::WIDTH/2-17, 29, 29, 5);
    _window->createWin(MAIN_WINDOW, 85, 3, 39, 37);
    // _window->createTxtInput(30, 40, 11, 40, 3); Change pseudo
    _window->createTxtInput(30, 40, 11, 40, 3, false);
    // _window->createTxtInput(30, 40, 21, 40, 3); Change question
    // _window->createTxtInput(30, 40, 26, 40, 3); Change answer
    _window->createObj(MAIN_WINDOW, _user_name, SIZE::WIDTH/2 - 10, 5, 1);
    _window->createObj(MAIN_WINDOW, "PASSWORD", 42, 10, 1);
    _window->createObj(MAIN_WINDOW, "SECRET QUESTION :", 46, 15, 1);  
    _window->createObj(MAIN_WINDOW, "ANSWER OF THE QUESTION :", 46, 19, 1);  
    _window->createObj(MAIN_WINDOW, "Friends request", 88, 2, 1);  
    // DYNAMIC  
    _window->createObj(MAIN_WINDOW, "question", 47, 17, 1);
    _window->createObj(MAIN_WINDOW, "answer", 47, 21, 1);
    _window->createObj(MAIN_WINDOW, "", 84, 5, 1);
    _window->createObj(MAIN_WINDOW, "", 84, 35, 1);
    _window->createObj(MAIN_WINDOW, "", 43, 28, 1);
}

void Menu::MyProfileMenuNcurses::display() {
    checkArrow();
    timer(69);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawBox(MAIN_WINDOW, 1);
    _window->drawBox(2, 1);
    _window->drawTxtInput();
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::MyProfileMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    MNAME where = MNAME::NOTHING;
    switch (_window->writeText(_choice-_window->getNbButtons())) {
        case KEY::UP:
            if (_choice==0)
                _choice=2;
            else if (_choice==1)
                _choice = _window->getNbButtons();
            else if (_choice == 2)
                _choice=1;
            else if (_choice > STATIC_BUTTON) {
                 if ((_choice-STATIC_BUTTON)-_scrolling== 0) {
                    _window->scrollButtons(STATIC_BUTTON, _window->getNbButtons()-1, true);
                    _scrolling--;
                }
                if (_choice-STATIC_BUTTON != _window->getNbButtons()-1 && _choice!=_window->getNbButtons())
                    _choice--;
            }
        break;
        case KEY::DOWN:
            if (_choice==1)
                _choice=2;
            else if (_choice == _window->getNbButtons())
                _choice=1;
            else if (_choice == 2)
                _choice=0;
            else if (_choice==_window->getNbButtons()+3)
                _choice=1;
            else if(_choice > 1) {
                if (_choice < _window->getNbButtons()-1)
                    _choice++;
                if ((_choice-STATIC_BUTTON)-_scrolling == DISPLAYED_BUTTON){
                    _window->scrollButtons(STATIC_BUTTON, _window->getNbButtons()-1, false);
                    _scrolling++;
                }
            }
        break;
        case KEY::LEFT:
            if (_choice==1 || _choice==_window->getNbButtons())
                _choice=0;
            else if (_choice > 4+_scrolling && _friends_waiting.size()>4)
                _choice=1;
            else if (_choice == 2)
                _choice = 0;
            else if (_choice!=0)
                _choice=_window->getNbButtons();
        break;
        case KEY::RIGHT:
            if (_choice == 0)
                _choice = 2;
            else if (!_friends_waiting.empty()) {
                if ((_choice == 1 || _choice==2) && _friends_waiting.size()>4) 
                    _choice = STATIC_BUTTON+_scrolling+5;
                else if (_choice == _window->getNbButtons()||_choice==1||_choice==2)
                    _choice = STATIC_BUTTON+_scrolling;
            }
        break;
        case KEY::ENTER:
            where = goTo();
        break;
        default:
            break;
    }
    _window->setHiglight(_choice);
    return where;
}

void Menu::MyProfileMenuNcurses::createButton() {
    size_t tmp_size = _friends_waiting.size() < DISPLAYED_BUTTON ? _friends_waiting.size() : DISPLAYED_BUTTON;
    for (size_t i = STATIC_BUTTON; i < tmp_size+STATIC_BUTTON; i++) { // 7 displayed
        _window->createButton(_friends_waiting[i-STATIC_BUTTON], 87, static_cast<int>(4+(i-STATIC_BUTTON)*5), 35, 5);
    }
    for (size_t i = DISPLAYED_BUTTON+STATIC_BUTTON; i < _friends_waiting.size()+STATIC_BUTTON; i++) { // 6 Hide
         _window->createButton(_friends_waiting[i-STATIC_BUTTON], 87, SIZE::HEIGHT, 35, 5);
    }
}

void Menu::MyProfileMenuNcurses::checkArrow() {
    if (_scrolling > 0) 
        _window->changeObject(_window->getNbObject()-3, "^\n|", 1);
    else
        _window->changeObject(_window->getNbObject()-3, " \n ", 1);
    if (_friends_waiting.size() > _scrolling+DISPLAYED_BUTTON)
        _window->changeObject(_window->getNbObject()-2, "|\nv", 1);
    else
        _window->changeObject(_window->getNbObject()-2, " \n ", 1);   
}

MNAME Menu::MyProfileMenuNcurses::goTo() noexcept {
    if (_choice == 0)
        return MNAME::SOCIAL_MENU; 
    else if (_choice == 1)
        changeInfo();
    else if (_choice == 2){
        setFriendName(_user_name);
        return MNAME::FRIEND_LEVEL_MENU;
    }
    else {
        setFriendName(_friends_waiting[_choice-STATIC_BUTTON]);
        return MNAME::PROFILE_FRIENDS;
    }
    return MNAME::NOTHING;
}

void Menu::MyProfileMenuNcurses::setUser(int score, char* question, char* answer, const char* pending_list) {
    _window->changeObject(_window->getNbObject()-5, question, 1);
    _window->changeObject(_window->getNbObject()-4, answer, 1);
    _window->changeObject(_stars+1, "Score: "+std::to_string(score), 1);
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
    createButton();
}

void Menu::MyProfileMenuNcurses::changeInfo() {
    if (_window->getText(0)[2] != '\0') {
        Menu::MyProfileMenu::changeInfo(_window->getText(0));
        _window->changeObject(_window->getNbObject()-1, "Password update !", 1);
    }
    else{
       _window->changeObject(_window->getNbObject()-1, "Password need at least 3 char", 1);   
    }
}