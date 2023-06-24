#include "MainMenuNcurses.hpp"

const std::string NAME_BTN[4] = {"P L A Y", "SOCIAL", "LEADERBOARD", "EXIT"};
const char* SPRITE_TXT = " __             .___________.____    ____ .______    _______        __   ___  \n"
"|  |            |           |\\   \\  /   / |   _  \\  |   ____|      /_ | |__ \\ \n"
"|  |      ______`---|  |----` \\   \\/   /  |  |_)  | |  |__    ______| |    ) |\n"
"|  |     |______|   |  |       \\_    _/   |   ___/  |   __|  |______| |   / / \n"
"|  `----.           |  |         |  |     |  |      |  |____        | |  / /_ \n"
"|_______|           |__|         |__|     | _|      |_______|       |_| |____|";

Menu::MainMenuNcurses::MainMenuNcurses() noexcept {
    initAll();
}

void Menu::MainMenuNcurses::initAll() noexcept {
    _window = new Menu::MenuMaker();
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    for (size_t i = 0; i < sizeof(NAME_BTN)/sizeof(NAME_BTN[0]); i++) {
        _window->createButton(NAME_BTN[i], 40, 15+5*static_cast<int>(i), 46, 5);
    }
    if (_client->getSound()->isMute())
        _window->createButton("MUTE", 45, 36, 36, 3);
    else
        _window->createButton("SOUND", 45, 36, 36, 3); 
    _window->createObj(MAIN_WINDOW ,SPRITE_TXT, 25, 3, 1);
}

void Menu::MainMenuNcurses::display(){
    timer(50);
    _window->drawObjects();
    _window->drawBox(MAIN_WINDOW, 1);
    _window->drawButtons();
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::MainMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    switch (_window->input()) {
        case KEY::UP:
            if (_choice > 0)
                _choice--;
            break;
        case KEY::DOWN:
            _choice++;
            if (_choice == 5)
                _choice = 4;
            break;
        case KEY::ENTER:
            if (_choice == 0){
                setFriendName("");
                return MNAME::ROOM_MENU;
            }
            else if (_choice == 1)
                return MNAME::SOCIAL_MENU;
            else if (_choice == 2)
                return MNAME::LEADERBOARD_MENU;
            else if (_choice == 3)
                return MNAME::EXIT_MENU;
            else if (_choice == 4) {
                _client->getSound()->toggleMute();
                if (_client->getSound()->isMute())
                    _window->changeButtons(4, strdup("MUTE"));
                else
                    _window->changeButtons(4, strdup("SOUND"));
            }
            break;
        default:
            break;
    }
    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}