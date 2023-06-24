    #include "RoomMenuNcurses.hpp"
#include "../ConnectionMenu/ConnectionInfo.hpp"

const int BUTTON_WIDTH = 30;
const int BUTTON_HEIGHT = 3;

const std::string NAME_TXT[] = {"COOP","DIFFICULTY", "DROP RATE", "FRIENDLY FIRE", "LIVES", "LEVEL"};
const char* ROOMSPRITE_TXT = " _____   ____   ____  __  __ \n"
 "|  __ \\ / __ \\ / __ \\|  \\/  |\n"
 "| |__) | |  | | |  | | \\  / |\n"
 "|  _  /| |  | | |  | | |\\/| |\n"
 "| | \\ \\| |__| | |__| | |  | |\n"
 "|_|  \\_ \\____/ \\____/|_|  |_|";

Menu::RoomMenuNcurses::RoomMenuNcurses() noexcept {
    initAll();
}

void Menu::RoomMenuNcurses::initAll() noexcept {
    /**
    *   Initializes the window,buttons,title and stars
    */
    _window = new Menu::MenuMaker();
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);

    decorate();
    _window->createObj(MAIN_WINDOW, ROOMSPRITE_TXT, 49, 1, 1);

    createButton();
    createText();

    startingHighlight();
}

void Menu::RoomMenuNcurses::display(){
    _window->drawObjects();
    _window->drawTxtInput();
    _window->drawButtons();
    _window->drawBox(MAIN_WINDOW, 1);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::RoomMenuNcurses::inputs() {
    /**
    *   A lot of 'if' because there is no symmetry
    */
    if (!_choice_array[_choice])
        _window->unsetHiglight(_choice);
    else
        _window->unsetHiglight(_choice, true);
    switch (_window->writeText(_choice-_window->getNbButtons())) {
        case KEY::UP:
            if(_choice==11)
                _choice-=5;
            else if (_choice==9 || _choice==10 || _choice==2 || _choice==12)
                _choice-=2;
            else if(_choice==1)
                _choice--;
            else if (_choice==_window->getNbButtons())
                _choice=9;
            else if (_choice==13)
                _choice=15;
            else if (_choice==15)
                _choice=_window->getNbButtons();
            else{
                if (_choice > 2)
                    _choice-=3;
            }
        break;

        case KEY::DOWN:
            if(_choice==6)
                _choice+=5;     
            else if(_choice==7||_choice==8||_choice==10)
                _choice+=2;
            else if(_choice==0||_choice==11)
                _choice++;
            else if (_choice==9)
                _choice=_window->getNbButtons();
            else if (_choice==_window->getNbButtons()||_choice==12)
                _choice=15;
            else if (_choice==15)
                _choice=13;
            else{
                _choice+=3;
                if (_choice >13)
                    _choice-=3;
            }
        break;

        case KEY::LEFT:
            if (_choice==12)
                _choice=_window->getNbButtons();
            else if (_choice==_window->getNbButtons())
                _choice=11;
            else if (_choice > 0 && _choice < _window->getNbButtons())
                _choice--;
        break;

        case KEY::RIGHT:
            if (_choice==_window->getNbButtons())
                _choice=12;
            else if (_choice==11)
                _choice=_window->getNbButtons();
            else if(_choice==15)
                _choice=13;  
            else if(_choice<14)
                _choice++;  
        break;

        case KEY::ENTER:
            if(_choice==0) {
                if (_friend_name[0]=='\0') {
                    Menu::ConnectionInfo::_is_user = false;
                    return MNAME::CONNECTION_MENU;
                }
                logoutPlayer2(getFriendName());
                _is_solo=Game::GameSettings::GameMode::Single;
                setFriendName("\0");
                _window->changeObject(_window->getNbObject()-1, " ", 1);
                _choice_array[0]=false;
                return MNAME::NOTHING;
            }
            else if (_choice < 12)
                setSettings();
            else if (_choice==12){
                // RECHERCHE NIVEAU
                _level_name = _window->getText(0);
                _level_name = _level_name + ".txt";
                auto* command = Game::CommandFactory::createLevel(_user_name, _level_name.c_str(), "", true);
                MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
                //_window->changeObject(_window->getNbObject()-2, _window->getText(0), 1);
            }
            else if (_choice==13){
                if (_level_name_correct){
                    if (_name_level != _level_name && _name_level == strdup("")){
                        RoomMenu::setLevelPlayed(_level_name);
                    }else{
                        _level_name = _name_level;
                    }
                    _level_name = "levels/" + _level_name;
                    if (_friend_name[0]!='\0')
                        _is_solo = Game::GameSettings::GameMode::Coop;
                    start();
                    return MNAME::GAME;
                }else{
                    _window->changeObject(_window->getNbObject()-2, "Enter name of a level", 1);
                }
                return MNAME::NOTHING;
            }
            else if(_choice==14) {
                logoutPlayer2(getFriendName());
                return MNAME::MAIN_MENU;
            }
            else if(_choice==15) {
                _level_name = DEFAULT_LEVEL;
                auto* command = Game::CommandFactory::createLevel(_user_name, _level_name.c_str(), "", true);
                MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
            }
            if (_choice <= 12)
                _choice_array[_choice]=true;//to keep the highlight
        
        break;
        default:
            break;
    }
    _window->setHiglight(_choice, true);
    return MNAME::NOTHING;
}

void Menu::RoomMenuNcurses::startingHighlight() noexcept {
    if (_friend_name[0]!='\0') {
        _window->setHiglight(0);
        _choice_array[0]=true;
    }
    _window->setHiglight(2);
    _window->setHiglight(5);
    _window->setHiglight(7);
    _window->setHiglight(10);
    _choice_array[2]=true;
    _choice_array[5]=true;
    _choice_array[7]=true;
    _choice_array[10]=true;
}

void Menu::RoomMenuNcurses::newHighlight(int first,int last){
    /**
    *   Used to "unset" the highlight on the same line than the new choice
    */
    for(int i=first;i<=last;i++){
        if(_choice_array[i]){
            _window->unsetHiglight(static_cast<size_t>(i));
            _choice_array[i]=false;
        }
    _window->setHiglight(_choice);
    }  
}

void Menu::RoomMenuNcurses::createButton() {
    /**
    *   Creates all the buttons, cant do it in a proper way since they aren't symmetrical
    */
    _window->createButton("PLAYER2 LOG IN", BUTTON_WIDTH,8,BUTTON_WIDTH,BUTTON_HEIGHT);

    _window->createButton("EASY", BUTTON_WIDTH, 12, BUTTON_WIDTH, BUTTON_HEIGHT);
    _window->createButton("NORMAL", BUTTON_WIDTH*2, 12, BUTTON_WIDTH, BUTTON_HEIGHT);
    _window->createButton("HARD", BUTTON_WIDTH*3, 12, BUTTON_WIDTH, BUTTON_HEIGHT);

    _window->createButton("A LOT", BUTTON_WIDTH, 16, BUTTON_WIDTH, BUTTON_HEIGHT);
    _window->createButton("NORMAL", BUTTON_WIDTH*2, 16, BUTTON_WIDTH, BUTTON_HEIGHT);
    _window->createButton("NONE", BUTTON_WIDTH*3, 16, BUTTON_WIDTH, BUTTON_HEIGHT);

    _window->createButton("NO", BUTTON_WIDTH, 20, BUTTON_WIDTH, BUTTON_HEIGHT);
    _window->createButton("YES", BUTTON_WIDTH*2, 20, BUTTON_WIDTH, BUTTON_HEIGHT);

    _window->createButton("3", BUTTON_WIDTH, 24, BUTTON_WIDTH, BUTTON_HEIGHT);
    _window->createButton("5", BUTTON_WIDTH*2, 24, BUTTON_WIDTH, BUTTON_HEIGHT);
    _window->createButton("10", BUTTON_WIDTH*3, 24, BUTTON_WIDTH, BUTTON_HEIGHT);
    _window->createButton(">", BUTTON_WIDTH*2+5, 28, BUTTON_WIDTH/4, BUTTON_HEIGHT);

    _window->createButton("P L A Y", static_cast<int>(static_cast<int>(SIZE::WIDTH/2)-(BUTTON_WIDTH*1.5)/2), 34,
                          static_cast<int>(BUTTON_WIDTH * 1.5), BUTTON_HEIGHT * 2);
    _window->createButton("BACK", 100, SIZE::HEIGHT-8, BUTTON_WIDTH/2, 5);
    _window->createButton("Campaign", BUTTON_WIDTH, 31, static_cast<int>(BUTTON_WIDTH / 1.5), 3);
    
    _window->createTxtInput(LEVEL_NAME_MAX_SIZE, BUTTON_WIDTH, 28, BUTTON_WIDTH+5, BUTTON_HEIGHT);
}

void Menu::RoomMenuNcurses::createText(){
    for (size_t i = 0; i < sizeof(NAME_TXT)/sizeof(NAME_TXT[0]); i++){
        _window->createObj(MAIN_WINDOW, NAME_TXT[i], 6, static_cast<int>(9+i*4), 1);
    }    
    if (_friend_name[0]!='\0')
        _is_solo=Game::GameSettings::GameMode::Coop;
    
    std::string tmp_friend = _friend_name;
    if(*getLevelPlayed() != '\0'){
        _level_name = getLevelPlayed();
        _level_name.erase(_level_name.end() - 4, _level_name.end());
        _window->createObj(MAIN_WINDOW, _level_name, BUTTON_WIDTH*2+14, 29, 1);
    }else{
        RoomMenu::setLevelPlayed(_level_name);
        _window->createObj(MAIN_WINDOW, "", BUTTON_WIDTH*2+14, 29, 1);
    }
    if (_is_solo == Game::GameSettings::GameMode::Coop){
        _window->createObj(MAIN_WINDOW, "P2: "+tmp_friend, BUTTON_WIDTH*2+1, 9, 1);
    }
    
}

void Menu::RoomMenuNcurses::decorate() noexcept {
   /**
   *   Creates the stars
   */
    _window->createObj(MAIN_WINDOW, "*", 4, 3, 1);
    _window->createObj(MAIN_WINDOW, "*", 12, 10, 1);
    _window->createObj(MAIN_WINDOW, "*", 20, 30, 1);
    _window->createObj(MAIN_WINDOW, "*", 110, 30, 1);
    _window->createObj(MAIN_WINDOW, "*", 90, 25, 1);
    _window->createObj(MAIN_WINDOW, "*", 96, 37, 1);
    _window->createObj(MAIN_WINDOW, "*", 80, 39, 1);
    _window->createObj(MAIN_WINDOW, "*", 30, 35, 1);
    _window->createObj(MAIN_WINDOW, "*", 110, 3, 1);
    _window->createObj(MAIN_WINDOW, "*", 101, 10, 1);
}

void Menu::RoomMenuNcurses::setSettings() noexcept {
    if(_choice<=3 && _choice>=1) {
        newHighlight(1,3);
        if (_choice==1)
            _difficulty=Game::GameSettings::Difficulty::Easy;
        else if (_choice==2)
            _difficulty=Game::GameSettings::Difficulty::Normal;
        else
            _difficulty=Game::GameSettings::Difficulty::Hard;
    }
    else if(_choice<=6 && _choice>=4) {
        newHighlight(4,6);
        if (_choice==4)
            _dropping_rate=Game::GameSettings::DropRate::High;
        else if (_choice==5)
            _dropping_rate=Game::GameSettings::DropRate::Average;
        else
            _dropping_rate=Game::GameSettings::DropRate::None;
    }
    else if(_choice==7||_choice==8) {
        newHighlight(7,8);
        if (_choice==7)
            _friendly_fire=Game::GameSettings::FriendlyFire::No;
        else if (_choice==8)
            _friendly_fire=Game::GameSettings::FriendlyFire::Yes;
    }
    else if(_choice==11 || _choice==10 || _choice==9){
        newHighlight(9,11);
        if (_choice==9)
            _lives=Game::GameSettings::HealthPoints::Few;
        else if (_choice==10)
            _lives=Game::GameSettings::HealthPoints::Some;
        else if (_choice == 11)
            _lives=Game::GameSettings::HealthPoints::Many;
    }
}

void Menu::RoomMenuNcurses::setTextError(bool work) noexcept{
    if (work) {
        _level_name_correct = false;
        _window->changeObject(_window->getNbObject()-1, "Level doesn't exist", 1);
    }else{
        _level_name_correct = true;
        setLevelPlayed(_level_name);
        _window->changeObject(_window->getNbObject()-1, "Level exist !", 1);
    }
}