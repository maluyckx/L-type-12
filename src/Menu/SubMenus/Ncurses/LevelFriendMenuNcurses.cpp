#include "LevelFriendMenuNcurses.hpp"
#include "../RoomMenu.hpp"

const int DISPLAYED_BUTTON = 7; // voir ici
const int STATIC_BUTTON = 1;

Menu::LevelFriendMenuNcurses::LevelFriendMenuNcurses() noexcept {
    initAll();
}

void Menu::LevelFriendMenuNcurses::initAll() noexcept { // full a changer
    getLevels(); 
    _window = new Menu::MenuMaker();
    _window->clearAll();
    _window->createWin(0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    decorate();
    std::string user_name = MenuInfo::getFriendName();
    user_name += "'s levels";
    _window->createObj(MAIN_WINDOW, user_name, WIDTH_WINDOW/2-5, 2, 1);
    _window->createButton("BACK", 5, 35, 10, 5); 
    _window->createWin(MAIN_WINDOW, 20, 3, 86, 37);
}

void Menu::LevelFriendMenuNcurses::display(){
    checkArrow();
    timer(60);
    _window->drawObjects();
    _window->drawButtons();
    _window->drawBox(2, 1);
    _window->drawBox(MAIN_WINDOW, 1);
    _window->updateWin(MAIN_WINDOW);
}

MNAME Menu::LevelFriendMenuNcurses::inputs() {
    _window->unsetHiglight(_choice);
    switch (_window->input()){
        case KEY::UP:
            if (_choice > 0){
                if (_choice > 1){
                    _choice -= 1;
                    if (_choice < STATIC_BUTTON + _scrolling){
                        _window->scrollButtons(STATIC_BUTTON, _window->getNbButtons()-1, true);
                        _scrolling -= 1;
                    }
                }
            }
            break;
        case KEY::DOWN:
            if (_choice > 0){
                if (_choice < _window->getNbButtons() - 1){
                    _choice += 1;
                    if (_choice > DISPLAYED_BUTTON + _scrolling){
                        _window->scrollButtons(STATIC_BUTTON, _window->getNbButtons()-1, false);
                        _scrolling += 1;
                    }
                }
            }
            break;
        case KEY::LEFT:
            if (_choice != 0)
                _choice = 0;
            break;
        case KEY::RIGHT:
            if (_choice == 0)
                _choice = 1;
            break;
        case KEY::ENTER:
            if (_choice == 0)
                return MNAME::SOCIAL_MENU;
            else{
                std::string level_name = _name_levels[_choice-1];
                level_name += ".txt"; 
                RoomMenu::setLevelPlayed(level_name);
                MenuInfo::setFriendName("");
                return MNAME::ROOM_MENU;
            }
            break;    
        default:
            break;
    }
    _window->setHiglight(_choice);
    return MNAME::NOTHING;
}

void Menu::LevelFriendMenuNcurses::createButtons() {
    size_t tmp_size = _list_levels.size() < DISPLAYED_BUTTON ? _list_levels.size() : DISPLAYED_BUTTON;
    for (size_t i = 0; i < tmp_size; i++) { // 6 displayed
        _window->createButton(_list_levels[i], 21, static_cast<int>(4+i*5), 83, 5);
    }
    for (size_t i = DISPLAYED_BUTTON; i < _list_levels.size(); i++) { // 6 Hide
         _window->createButton(_list_levels[i], 21, SIZE::HEIGHT, 83, 5);
    }
}


// DECOOOOOOOOO
void Menu::LevelFriendMenuNcurses::decorate() noexcept {
    _window->createWin(MAIN_WINDOW, 0, 0, SIZE::WIDTH, SIZE::HEIGHT);
    _window->createObj(DECORATE_WINDOW, "*", 107, 4, 1);
    _window->createObj(DECORATE_WINDOW, "+", 107, 37, 1);
    _stars=2;
}

void Menu::LevelFriendMenuNcurses::checkArrow() {
    if (_scrolling > 0) 
        _window->changeObject(_window->getNbObject()-3, "^\n|", 1);
    else
        _window->changeObject(_window->getNbObject()-3, " \n ", 1);
    if (_list_levels.size() > _scrolling+DISPLAYED_BUTTON)
        _window->changeObject(_window->getNbObject()-2, "|\nv", 1);
    else
        _window->changeObject(_window->getNbObject()-2, " \n ", 1);   
}

void Menu::LevelFriendMenuNcurses::animation() noexcept {
    _window->changeObject(static_cast<unsigned long>(std::rand()) / ((RAND_MAX + 1u) / _stars), "+", 1);
    _window->changeObject(static_cast<unsigned long>(std::rand()) / ((RAND_MAX + 1u) / _stars), "*", 1);
}

void Menu::LevelFriendMenuNcurses::getLevels() {
    auto* command = Game::CommandFactory::userLevelList(MenuInfo::getFriendName()); 
    MenuInfo::getClient()->getConnection()->getSendingCommandQueue()->push(command);
}

void Menu::LevelFriendMenuNcurses::setLevelList(const char* levels, int* level_like, unsigned nb_levels) noexcept {
    for (size_t i = 0; i < nb_levels; i++) {
        char tmp_str[LEVEL_NAME_MAX_SIZE];
        int count_tmp = 0;
        for (size_t j = i*LEVEL_NAME_MAX_SIZE; j < LEVEL_NAME_MAX_SIZE*(i+1); j++) {
            tmp_str[count_tmp++] = levels[j];
        }
        std::string res = tmp_str;
        _name_levels.emplace_back(res);
        res += " : ";
        res += std::to_string(level_like[i]);
        res += " <3";
        _list_levels.emplace_back(res);
    }
    if (_list_levels.empty()){ 
        _window->createObj(MAIN_WINDOW, static_cast<std::string>(MenuInfo::getFriendName()) + " didn't create a level yet", WIDTH_WINDOW/2-15, 2, 1);
    }
    createButtons();
}