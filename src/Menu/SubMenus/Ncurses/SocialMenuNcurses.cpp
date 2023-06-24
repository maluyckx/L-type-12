#include "SocialMenuNcurses.hpp"

const int DISPLAYED_BUTTON = 7;
 const int STATIC_BUTTON=3;
const char* SPRITE_TEXT = "  _____  ____   _____ _____          _      \n"
 " / ____|/ __ \\ / ____|_   _|   /\\   | |     \n"
 "| (___ | |  | | |      | |    /  \\  | |     \n"
 " \\___ \\| |  | | |      | |   / /\\ \\ | |     \n"
 " ____) | |__| | |____ _| |_ / ____ \\| |____ \n"
 "|_____/ \\____/ \\_____|_____/_/    \\_\\______|";

Menu::SocialMenuNcurses::SocialMenuNcurses() noexcept {
    initAll();
}

void Menu::SocialMenuNcurses::initAll() noexcept {
    requestFriendList();
    _window = new Menu::MenuMaker();
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    _window->createButton("BACK", 5, 35, 10, 5);
    _window->createButton("SEARCH", 50, 24, 8, 3);
    _window->createButton(_user_name, 5, 18, 50, 5);
    _window->createTxtInput(35, 5, 24, 43, 3);
    _window->createWin(MAIN_WINDOW, 77, 2, 44, 37);
    _window->createObj(MAIN_WINDOW, SPRITE_TEXT, 15, 3, 1);
    _window->createObj(MAIN_WINDOW, "", 76, 5, 1);
    _window->createObj(MAIN_WINDOW, "", 76, 35, 1);
    _window->createObj(MAIN_WINDOW, "Searching", 6, 28, 1);
}

void Menu::SocialMenuNcurses::display(){
    checkArrow();
    timer(60);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawTxtInput();
    _window->drawBox(2, 1);
    _window->drawBox(MAIN_WINDOW, 1);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::SocialMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    MNAME where = MNAME::NOTHING;
    switch (_window->writeText(_choice-_window->getNbButtons())) {
        case KEY::UP:
            if (_choice == 0)
                _choice = _window->getNbButtons();
            else if (_choice == 1)
                _choice = 2;
            else if (_choice==_window->getNbButtons())
                _choice = 2;
            else if (_choice > STATIC_BUTTON) {
                if ((_choice-(STATIC_BUTTON))-_scrolling == 0) {
                    _window->scrollButtons(STATIC_BUTTON, _window->getNbButtons()-1, true);
                    _scrolling--;
                }
                if (_choice-(STATIC_BUTTON) != _window->getNbButtons()-1)
                    _choice--;
            }
        break;
        case KEY::DOWN:
            if (_choice == 1) 
                _choice = 0;
            else if (_choice == 2) 
                _choice = _window->getNbButtons();
            else if(_choice < _window->getNbButtons()-1 && _choice >= STATIC_BUTTON) {
                _choice++;
                if (_choice-_scrolling == DISPLAYED_BUTTON+STATIC_BUTTON){
                    _window->scrollButtons(STATIC_BUTTON, _window->getNbButtons()-1, false);
                    _scrolling++;
                }
            }
            else if (_choice==_window->getNbButtons())
                _choice = 0;
        break;
        case KEY::LEFT:
            if (_choice == 1)
                _choice = _window->getNbButtons();
            else  if (_choice >= STATIC_BUTTON && _choice < _friends_list.size()+STATIC_BUTTON) {
                if ((_choice-STATIC_BUTTON)-_scrolling <= DISPLAYED_BUTTON/2)
                    _choice = 2;
                else if ((_choice-STATIC_BUTTON)-_scrolling == 6)
                    _choice = 0;
                else
                    _choice = 1;
            }
        break;
        case KEY::RIGHT:
            if (_friends_list.size() > 3 && _choice == 1)
                _choice = _scrolling+3+STATIC_BUTTON;
            else if (_friends_list.size() > 6 && _choice == 0)
                _choice = _scrolling+6+STATIC_BUTTON;
            else if (_choice==_window->getNbButtons())
                _choice = 1;
            else
                _choice = _scrolling+STATIC_BUTTON;
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

void Menu::SocialMenuNcurses::createButtons() {
    size_t tmp_size = _friends_list.size() < DISPLAYED_BUTTON ? _friends_list.size() : DISPLAYED_BUTTON;
    for (size_t i = 0; i < tmp_size; i++) { // 6 displayed
        _window->createButton(_friends_list[i], 79, static_cast<int>(3+i*5), 40, 5);
    }
    for (size_t i = DISPLAYED_BUTTON; i < _friends_list.size(); i++) { // 6 Hide
         _window->createButton(_friends_list[i], 79, SIZE::HEIGHT, 40, 5);
    }
}

MNAME Menu::SocialMenuNcurses::goTo() noexcept {
    if (_choice==1) {
        searchFriend();
        return MNAME::NOTHING;
    }else if (_choice==0){
        return MNAME::MAIN_MENU;
    }else if (_choice== 2){
        return MNAME::MY_PROFILE_MENU;
    }else {
        setFriendName(_friends_list[_choice-STATIC_BUTTON]);
        return MNAME::PROFILE_FRIENDS;
    }
}

// DECOOOOOOOOO
void Menu::SocialMenuNcurses::decorate() noexcept {
    _window->createWin(MAIN_WINDOW, 0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    _window->createObj(DECORATE_WINDOW, "*", 2, 2, 1);
    _window->createObj(DECORATE_WINDOW, "+", 3, 15, 1);
    _window->createObj(DECORATE_WINDOW, "*", 10, 16, 1);
    _window->createObj(DECORATE_WINDOW, "*", 40, 2, 1);
    _window->createObj(DECORATE_WINDOW, "+", 40, 12, 1);
    _window->createObj(DECORATE_WINDOW, "*", 35, 13, 1);
    _window->createObj(DECORATE_WINDOW, "*", 60, 14, 1);
    _window->createObj(DECORATE_WINDOW, "*", 63, 5, 1);
    _window->createObj(DECORATE_WINDOW, "*", 4, 6, 1);
    _window->createObj(DECORATE_WINDOW, "+", 10, 30, 1);
    _window->createObj(DECORATE_WINDOW, "+", 38, 25, 1);
    _window->createObj(DECORATE_WINDOW, "*", 50, 34, 1);
    _window->createObj(DECORATE_WINDOW, " * \n*+*\n * ", 43, 32, 1);
    _window->createObj(DECORATE_WINDOW, "/-\\\n\\_/", 69, 40, 1);
    _stars=12;
}

void Menu::SocialMenuNcurses::checkArrow() {
    if (_scrolling > 0) 
        _window->changeObject(_window->getNbObject()-3, "^\n|", 1);
    else
        _window->changeObject(_window->getNbObject()-3, " \n ", 1);
    if (_friends_list.size() > _scrolling+DISPLAYED_BUTTON)
        _window->changeObject(_window->getNbObject()-2, "|\nv", 1);
    else
        _window->changeObject(_window->getNbObject()-2, " \n ", 1);   
}

void Menu::SocialMenuNcurses::animation() noexcept {
    _window->changeObject(static_cast<unsigned long>(std::rand()) / ((RAND_MAX + 1u) / _stars), "+", 1);
    _window->changeObject(static_cast<unsigned long>(std::rand()) / ((RAND_MAX + 1u) / _stars), "*", 1);
    _ship_frame++;
    if (_ship_frame==4) {
        _window->move(_stars+1, _window->getPos(_stars+1, true), _window->getPos(_stars+1, false)-1);
        if (_window->getPos(_stars+1, false)==-4)
            _window->move(_stars+1, _window->getPos(_stars+1, true), SIZE::HEIGHT);
        _ship_frame=0;
    }
}

 void Menu::SocialMenuNcurses::setFriendsList(const char* fl, unsigned count_friends) noexcept {
     Menu::SocialMenu::setFriendsList(fl, count_friends);
     createButtons();
 }

 void Menu::SocialMenuNcurses::searchFriend() {
     if (strcmp(_user_name, _window->getText(0))!=0) {
        auto* command = Game::CommandFactory::userData(_window->getText(0));
        _client->getConnection()->getSendingCommandQueue()->push(command);
        setFriendName(_window->getText(0));
     }else {
         _window->changeObject(_window->getNbObject()-1, "Why are you looking for you ?", 1);
     }
 }

 void Menu::SocialMenuNcurses::setSearchText() {
     std::string tmp_str = _window->getText(0);
    _window->changeObject(_window->getNbObject()-1, tmp_str+" does not exist !", 1);
 }
