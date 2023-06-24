#include "LeaderboardMenuNcurses.hpp"

const std::string LEADERBOARD_NAME_TXT[5] = {"RANK","PSEUDO", "SCORE"};

const size_t NB_LEADERBOARD=20;

const char* LEADERBOARD_SPRITE_TXT = "  _____  _____ ____  _____  ______ \n"
 " / ____|/ ____/ __ \\|  __ \\|  ____|\n"
 "| (___ | |   | |  | | |__) | |__   \n"
 " \\___ \\| |   | |  | |  _  /|  __|  \n"
 " ____) | |___| |__| | | \\ \\| |____ \n"
 "|_____/ \\_____\\____/|_|  \\_\\______|";

const size_t LEADERBOARD_WINDOW=1;
const size_t MY_SCORE_WINDOW=2;

Menu::LeaderboardMenuNcurses::LeaderboardMenuNcurses() noexcept{
    initAll();
}

void Menu::LeaderboardMenuNcurses::initAll() noexcept {
    /**
    *   Initializes the window, buttons, title, leaderboard and stars
    */
    _window = new Menu::MenuMaker();
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    createWin();
    createButton();
    decorate();
    _window->createObj(MAIN_WINDOW, LEADERBOARD_SPRITE_TXT, 46, 3, 1);
    createObjects();
    requestLeaderboard();
}

void Menu::LeaderboardMenuNcurses::display() {
    // showMyScore();
    _window->drawBox(LEADERBOARD_WINDOW,1);
    _window->drawBox(MY_SCORE_WINDOW,1);
    _window->drawButtons();
    _window->drawObjects();

    _window->drawBox(MAIN_WINDOW, 1);
    _window->updateWin(LEADERBOARD_WINDOW);
    _window->updateWin(MY_SCORE_WINDOW);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::LeaderboardMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);

    switch (_window->input()) {
        case KEY::UP:
            if (_choice == 1)
                _choice = 0;
        break;
        case KEY::DOWN:
            if (_choice == 0)
                _choice = 1;
        break;
        case KEY::LEFT:
            if (_choice == 1)
                _choice = 0;
        break;
        case KEY::RIGHT:
            if (_choice == 0)
                _choice = 1;
        break;
        case KEY::ENTER:
            if(_choice==1)
                return MNAME::MAIN_MENU;
            else if(_choice==0)
                requestLeaderboard();
        break;
        default:
            break;
    }

    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

void Menu::LeaderboardMenuNcurses::createButton(){
    /**
    *   Creates buttons 
    */
    _window->createButton("REFRESH",104,5,19,5);
    _window->createButton("BACK", 104, 35, 18, 5);
}

void Menu::LeaderboardMenuNcurses::createWin(){
    /**
    *   Creates 2 sub windows
    */
    _window->createWin(MAIN_WINDOW,3,12,120,23);
    _window->createWin(MAIN_WINDOW,3,35,100,6);
}

void Menu::LeaderboardMenuNcurses::decorate() noexcept {
   /**
   *    Creates stars
   */
    _window->createObj(MAIN_WINDOW, "*", 4, 3, 1);
    _window->createObj(MAIN_WINDOW, "*", 30, 4, 1);
    _window->createObj(MAIN_WINDOW, "*", 14, 10, 1);
    _window->createObj(MAIN_WINDOW, "*", 68, 9, 1);
    _window->createObj(MAIN_WINDOW, "*", 95, 8, 1);
    _window->createObj(MAIN_WINDOW, "*", 115, 2, 1);
}

void Menu::LeaderboardMenuNcurses::createObjects(){
    // STATIC TEXTE
    for (size_t i = 0; i < sizeof(LEADERBOARD_NAME_TXT)/sizeof(LEADERBOARD_NAME_TXT[0]); i++){
        _window->createObj(MAIN_WINDOW, LEADERBOARD_NAME_TXT[i], static_cast<int>(8+i*45), 11, 1);
    }
    _window->createObj(MY_SCORE_WINDOW, "MY SCORE:", 2, 2, 1);

    // DYNAMIC
    for (size_t i = 0; i < 3; i++){
        _window->createObj(MY_SCORE_WINDOW, "", static_cast<int>(13+i*37), 2, 1);
    }
    for (int i = 0; i < 20; ++i){
        _window->createObj(LEADERBOARD_WINDOW, "", 8, 1+i, 1);
        _window->createObj(LEADERBOARD_WINDOW, "", static_cast<int>(7+45), 1+i, 1);
        _window->createObj(LEADERBOARD_WINDOW, "", static_cast<int>(5+90), 1+i, 1);
    }   
}

void Menu::LeaderboardMenuNcurses::cleanLeaderboard() {
    for (size_t i = _window->getNbObject(); i < NB_LEADERBOARD*3; i--) {
        _window->changeObject(i, "", 1);
    }
    _window->updateWin(LEADERBOARD_WINDOW);
}

void Menu::LeaderboardMenuNcurses::setLeaderboard(const char* usernames_list, int* scores_list) noexcept {
    cleanLeaderboard();
    size_t starting = _window->getNbObject()-(NB_LEADERBOARD*3);
    int position = 1;
    for (size_t i = 0; i < 20; i++) {
        if (usernames_list[i*32]!='\0') {
            char tmp_str[32];
            int count_tmp=0;
            for (size_t j = i*32; j < 32*(i+1); j++) {
                tmp_str[count_tmp++] = usernames_list[j];
            } 
            _window->changeObject(starting + 3*i, std::to_string(position++), 1);
            _window->changeObject(starting + 3*i+1, tmp_str, 1);
            _window->changeObject(starting + 3*i+2, std::to_string(scores_list[i]), 1);
        }
    }
}

void Menu::LeaderboardMenuNcurses::setLeaderboardMy(int my_score) noexcept {
    size_t starting = _window->getNbObject()-(NB_LEADERBOARD*3)-3;
    _window->changeObject(starting, "", 1);
    _window->changeObject(starting+1, _user_name, 1);
    _window->changeObject(starting+2, std::to_string(my_score), 1);
}