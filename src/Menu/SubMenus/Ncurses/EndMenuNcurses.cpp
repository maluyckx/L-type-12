#include "EndMenuNcurses.hpp"
#include "../RoomMenu.hpp"
#include <string>

const std::string NAME_DISPLAY[2] = {"GAME OVER\n  LOOSER", "GG YOU WIN"}; 
const char* SPRITE_CROWN=
"  _.+._\n"
"(^\\/^\\/^)\n"
" \\@*@*@/\n"
" {_____}\n";
const char* SPRITE_POOP=
"  ) _   )\n"
"   ( \\_\n"
" _(_\\ \\)__\n"
"(____\\___))";

Menu::EndMenuNcurses::EndMenuNcurses(int level_remain, int score_user, int score_friend) noexcept:
                        EndMenu(level_remain, score_user, score_friend) {
    initAll();
}

void Menu::EndMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    Menu::MenuMaker::clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    _window->createButton("ROOM", 5, SIZE::HEIGHT-8, 40, 5);
    _window->createButton("LEADERBOARD", 80, SIZE::HEIGHT-8, 40, 5);
    if (!isLevelACampaign()){
        _window->createButton("LIKE", 100, 3, 20, 5);
    }else{
        _window->createButton("", 400, 3, 20, 5);
    }
    _window->createObj(MAIN_WINDOW, NAME_DISPLAY[_level_remain<=0], SIZE::WIDTH/2 - 5, 3, 1);
    if (_friend_name[0] != '\0') {
        if (_score_user < _score_friend) {
            _window->createObj(MAIN_WINDOW, _user_name, SIZE::WIDTH/2 - 3, 16, 1);
            _window->createObj(MAIN_WINDOW, "Score: "+std::to_string(_score_user), SIZE::WIDTH/2 - 5, 18, 1);
            _window->createObj(MAIN_WINDOW, _friend_name, SIZE::WIDTH/2 - 3, 11, 1);
            _window->createObj(MAIN_WINDOW, "Score: "+std::to_string(_score_friend), SIZE::WIDTH/2 - 5, 13, 1);
        } else {
            _window->createObj(MAIN_WINDOW, _user_name, SIZE::WIDTH/2 - 3, 11, 1);
            _window->createObj(MAIN_WINDOW, "Score: "+std::to_string(_score_user), SIZE::WIDTH/2 - 5, 13, 1);
            _window->createObj(MAIN_WINDOW, _friend_name, SIZE::WIDTH/2 - 3, 16, 1);
            _window->createObj(MAIN_WINDOW, "Score: "+std::to_string(_score_friend), SIZE::WIDTH/2 - 5, 18, 1);
        }
    }else {
        _window->createObj(MAIN_WINDOW, _user_name, SIZE::WIDTH/2 - 3, 11, 1);
        _window->createObj(MAIN_WINDOW, "Score: "+std::to_string(_score_user), SIZE::WIDTH/2 - 5, 13, 1);
    }

    if (!isLevelACampaign()){
        auto* command = Game::CommandFactory::isliked(_user_name, RoomMenu::getLevelPlayed());
        MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
    }
}

void Menu::EndMenuNcurses::display(){
    timer(50);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawBox(MAIN_WINDOW, 0);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::EndMenuNcurses::inputs() {
     _window->unsetHiglight(_choice);
    switch (Menu::MenuMaker::input()) {
        case KEY::LEFT:
            _choice = 0;
            break;
        case KEY::RIGHT:
            _choice = 1;
            break;
        case KEY::UP:
            if(!isLevelACampaign()) _choice = 2;
            break;
        case KEY::DOWN:
            _choice = 1;
            break;
        case KEY::ENTER:
            if (_choice == 0) {
                quitSound();
                return MNAME::ROOM_MENU; 
            }
            else if (_choice == 1) {
                quitSound();
                logoutPlayer2(getFriendName());
                return MNAME::LEADERBOARD_MENU;
            }else if (_choice == 2){
                Game::Command* command;
                if (_is_like){
                    _window->changeButtons(2, strdup("DISLIKE"));
                    command = Game::CommandFactory::likeLevel(_user_name, RoomMenu::getLevelPlayed());
                    _is_like = false;
                }else{
                    _window->changeButtons(2, strdup("LIKE"));
                    command = Game::CommandFactory::likeLevel(_user_name, RoomMenu::getLevelPlayed());
                    _is_like = true;
                }
                MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
            }
            break;
        default:
            break;  
    }
    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

void Menu::EndMenuNcurses::decorate() noexcept {
    Menu::BaseMenuNcurses::decorate();
    if (_level_remain<=0)
        _window->createObj(DECORATE_WINDOW, SPRITE_CROWN, SIZE::WIDTH/2 - 4, 6, 1);
    else {
        _window->createObj(DECORATE_WINDOW, SPRITE_POOP, SIZE::WIDTH/2 - 4, 6, 1);
        _window->createObj(DECORATE_WINDOW, std::to_string(_level_remain)+" remaining levels", SIZE::WIDTH/2 - 6, 20, 1);
    }
}

void Menu::EndMenuNcurses::animation() noexcept {
    Menu::BaseMenuNcurses::animation();
}

void Menu::EndMenuNcurses::setButtonLike(bool already_liked){
    if (already_liked){
        // _window->createObj(MAIN_WINDOW, "You already liked it", SIZE::WIDTH - 25, 2, 1);
        _window->changeButtons(2, strdup("DISLIKE"));
        _is_like=false;
        // _window->createObj(MAIN_WINDOW, "LIKE (BTN)", SIZE::WIDTH - 25, 2, 1);
        // _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML-480.f,  170.f, 430.f, 85.f, "sprites/button/simple_grey", "LIKE", 33, COLORS_SFML::WHITE));    // Même position car jamais les deux en même temps
    } else {
        _window->changeButtons(2, strdup("LIKE"));
        _is_like=true;
    }
}