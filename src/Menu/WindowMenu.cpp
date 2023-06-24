#include "WindowMenu.hpp"
#include "../Game/Client/Window.hpp"
#include <typeinfo>

// NCURSES INIT
void Menu::WindowMenu::init() {
    if (_is_graphical) {
        SFML::Texte::setFont();
        _mn = new Menu::ConnectionMenuSFML(_client);
        // _mn = new Menu::EndMenuSFML(true, 555, 0);
    } else {
        Menu::Window::ncursesInit();
        Menu::Window::ncursesColors();
        _mn = new Menu::ConnectionMenuNcurses(_client);
    }
}

void Menu::WindowMenu::display() {
    _mn->display();
}

void Menu::WindowMenu::input() {
    // if the input change the actual menu, it navigate, else if doesn't do anythings
    switch (_mn->inputs()) {
        case MNAME::EXIT_MENU:
            _state = false;
            _mn->quitMenu();
            break;
        case MNAME::GAME:
            _client->getWindow()->setState(Game::Client::WindowState::InGame);
            _state = false;
            break;
        case MNAME::MAIN_MENU:
            delete _mn;
            if (_is_graphical)
                _mn = new Menu::MainMenuSFML();
            else 
                _mn = new Menu::MainMenuNcurses();
            break;
        case MNAME::ROOM_MENU:
            delete _mn;
            if (_is_graphical)
                _mn = new Menu::RoomMenuSFML();
            else 
                _mn = new Menu::RoomMenuNcurses();
            break;
        case MNAME::SOCIAL_MENU:
            delete _mn;
            _menu_type=MNAME::SOCIAL_MENU;
            if (_is_graphical)
                _mn = new Menu::SocialMenuSFML();
            else 
                _mn = new Menu::SocialMenuNcurses();
            break;
        case MNAME::LEADERBOARD_MENU:
            delete _mn;
            _menu_type=MNAME::LEADERBOARD_MENU;       
            if (_is_graphical)
                _mn = new Menu::LeaderboardMenuSFML();
            else 
                _mn = new Menu::LeaderboardMenuNcurses();
            break;
        case MNAME::MY_PROFILE_MENU:
            delete _mn;
            _menu_type=MNAME::MY_PROFILE_MENU;
            if (_is_graphical)
                _mn = new Menu::MyProfileMenuSFML();
            else 
                _mn = new Menu::MyProfileMenuNcurses();
            break;
        case MNAME::PROFILE_FRIENDS:
            delete _mn;
            _menu_type=MNAME::PROFILE_FRIENDS;
            if (_is_graphical)
                _mn = new Menu::FriendProfileMenuSFML();
            else 
                _mn = new Menu::FriendProfileMenuNcurses();
            break;
        case MNAME::CONNECTION_MENU:
            delete _mn;
            if (_is_graphical)
                _mn = new Menu::ConnectionMenuSFML();
            else
                _mn = new Menu::ConnectionMenuNcurses();
            break;
        case MNAME::LOGIN_MENU:
            delete _mn;
            if (_is_graphical)
                _mn = new Menu::LoginMenuSFML();
            else 
                _mn = new Menu::LoginMenuNcurses();
            break;
        case MNAME::REGISTER_MENU:
            delete _mn;
            if (_is_graphical)
                _mn = new Menu::RegisterMenuSFML();
            else 
                _mn = new Menu::RegisterMenuNcurses();
            break;
        case MNAME::USER_NAME_VERIFICATION_MENU:
            delete _mn;
            if (_is_graphical)
                _mn = new Menu::UserNameVerificationMenuSFML();
            else 
                _mn = new Menu::UserNameVerificationMenuNcurses();
            break;
        case MNAME::LEVEL_MAIN_MENU :
            delete _mn;
            _mn = new Menu::LevelMainMenu();
            break;
        case MNAME::LEVEL_CHOICE_SETTINGS_MENU:
            delete _mn;
            _mn = new Menu::LevelChoiceSettingsMenu();
            break;
        case MNAME::LEVEL_CREATE_MENU :
            delete _mn;
            _mn = new Menu::LevelCreateMenu();
            break;
        case MNAME::FRIEND_LEVEL_MENU : 
            delete _mn;
            if (_is_graphical)
                _mn = new Menu::LevelFriendMenuSFML();
            else
                _mn = new Menu::LevelFriendMenuNcurses();
            break;
        default:
            break;
    }
}

bool Menu::WindowMenu::getState() const noexcept {
    return _state;
}

 void Menu::WindowMenu::setRegister(bool work) noexcept {
    if (work) {
        delete _mn;
        if (Menu::ConnectionInfo::_is_user) {
            if (_is_graphical)
                _mn = new Menu::MainMenuSFML();
            else 
                _mn = new Menu::MainMenuNcurses();
        }
        else {
            if (_is_graphical)
                _mn = new Menu::RoomMenuSFML();
            else 
                _mn = new Menu::RoomMenuNcurses();
        }
    }else {
        dynamic_cast<Menu::ConnectionInfo*>(_mn)->setTextError();
    }
 }

 void Menu::WindowMenu::setLogin(bool work) noexcept {
     if (work) {
        delete _mn;
        if (Menu::ConnectionInfo::_is_user) {
            if (_is_graphical)
                _mn = new Menu::MainMenuSFML();
            else 
                _mn = new Menu::MainMenuNcurses();
        }
        else {
            if (_is_graphical)
                _mn = new Menu::RoomMenuSFML();
            else 
                _mn = new Menu::RoomMenuNcurses();
        }
     }else {
        dynamic_cast<Menu::ConnectionInfo*>(_mn)->setTextError();
     }
 }

void Menu::WindowMenu::secretQuestion(char* question_str) noexcept {
    if (question_str[0] != '\0') {
        delete _mn;
        if (_is_graphical)
            _mn = new Menu::QuestionVerificationMenuSFML(question_str);
        else
            _mn = new Menu::QuestionVerificationMenuNcurses(question_str);
    } else {
        dynamic_cast<Menu::ConnectionInfo*>(_mn)->setTextError();
    }
}

void Menu::WindowMenu::secretAnswer(bool work) noexcept {
    if (work) {
        delete _mn;
        if (_is_graphical)
            _mn = new Menu::NewPasswordMenuSFML();
        else
            _mn = new Menu::NewPasswordMenuNcurses();
    } else {
        dynamic_cast<Menu::ConnectionInfo*>(_mn)->setTextError();
    }
}

void Menu::WindowMenu::socialMenuSetup(char* friends_list, unsigned count_friends) noexcept {
    dynamic_cast<Menu::SocialMenu*>(_mn)->setFriendsList(friends_list, count_friends);
}

void Menu::WindowMenu::friendLevelMenuSetup(char* levels_list, int* levels_like_list, unsigned level_count) noexcept {
    if (_is_graphical)
        dynamic_cast<Menu::LevelFriendMenuSFML*>(_mn)->setLevelList(levels_list, levels_like_list ,level_count);
    else
        dynamic_cast<Menu::LevelFriendMenuNcurses*>(_mn)->setLevelList(levels_list, levels_like_list ,level_count);
}

void Menu::WindowMenu::findFriend(bool is_existing) noexcept {
    if (is_existing) {
        delete _mn;
        _menu_type=MNAME::PROFILE_FRIENDS;
        if (_is_graphical)
            _mn = new Menu::FriendProfileMenuSFML();
        else
            _mn = new Menu::FriendProfileMenuNcurses();
    } else {
         dynamic_cast<Menu::SocialMenu*>(_mn)->setSearchText();
    }
}

void Menu::WindowMenu::setLeaderboard(char* usernames_list, int* scores_list) noexcept {
    dynamic_cast<Menu::LeaderboardMenu*>(_mn)->setLeaderboard(usernames_list, scores_list);
}

void Menu::WindowMenu::setMyLeaderboard(int my_score) noexcept {
    dynamic_cast<Menu::LeaderboardMenu*>(_mn)->setLeaderboardMy(my_score);
}

void Menu::WindowMenu::setProfileUser(int score, char* question, char* answer, char* pending_list) noexcept {
    dynamic_cast<Menu::MyProfileMenu*>(_mn)->setUser(score, question, answer, pending_list);
}

void Menu::WindowMenu::setFriendInfo(FRIEND_STATE state, int score) noexcept {
    dynamic_cast<Menu::FriendProfileMenu*>(_mn)->setFriendInfo(state, score);
}

void Menu::WindowMenu::setEndMenu(int end_code, int score_user, int score_friend) noexcept {
    delete _mn;
    _client->getSound()->isWinner(end_code<=0);
    if (_is_graphical)
        _mn = new Menu::EndMenuSFML(end_code, score_user, score_friend);
    else
        _mn = new Menu::EndMenuNcurses(end_code, score_user, score_friend);
}

void Menu::WindowMenu::setLevelChoiceNameMenu(bool work) {
    // TODO: Trouver mieux que ce try-catch I (Roggeman serait-il fier de nous ?)
    try{
        if (typeid(*_mn) == typeid(*dynamic_cast<Menu::LevelChoiceSettingsMenu*>(_mn))){ // Check si on était bien dans le menu de choix d'options de niveau
            if (work) {
                delete _mn;
                if (_is_graphical)
                    _mn = new Menu::LevelCreateMenu();
            }else {
                dynamic_cast<Menu::ConnectionInfo*>(_mn)->setTextError();
            }
        }
    }catch(const std::exception& e){
    }
 }

void Menu::WindowMenu::setLevelCreateMenu(bool work) noexcept {
    if (work) {
        delete _mn;
        if (_is_graphical)
            _mn = new Menu::LevelMainMenu();
    }else {
        dynamic_cast<Menu::ConnectionInfo*>(_mn)->setTextError();
    }
 }

void Menu::WindowMenu::setLevelRoomMenu(bool work) {
    // TODO: Trouver mieux que ce try-catch II (Roggeman re-serait-il fier de nous ?)
    try{
        if (_is_graphical){
            if (typeid(*_mn) == typeid(*dynamic_cast<Menu::RoomMenuSFML*>(_mn))){ // Check si on était bien dans la roomMenu
                    dynamic_cast<Menu::RoomMenuSFML*>(_mn)->setTextError(work);
            }
        }else{
            if (typeid(*_mn) == typeid(*dynamic_cast<Menu::RoomMenuNcurses*>(_mn))){ // Check si on était bien dans la roomMenu
                // TODO: Rajouter message d'erreur pour ncurses aussi
                dynamic_cast<Menu::RoomMenuNcurses*>(_mn)->setTextError(work);
            }
        }
    }catch(const std::exception& e){
    }
 }

// void Menu::WindowMenu::setSFMLLevelFriendMenuSFML(char* friend){
//     dynamic_cast<Menu::SFMLLevelFriendMenuSFML*>(_mn)->setFriend(friend);
// }

// void Menu::WindowMenu::myFriendLevelMenuSetup(char* levels_list, int* levels_like_list, unsigned level_count) noexcept {
//     dynamic_cast<Menu::SFMLLevelFriendMenuSFML*>(_mn)->setFriendLevelList(levels_list, levels_like_list ,level_count);
// }

void Menu::WindowMenu::setButtonLikeEndMenu(bool already_liked) {
    dynamic_cast<Menu::EndMenu*>(_mn)->setButtonLike(already_liked);
}

void Menu::WindowMenu::setMenuError(char* str_error) noexcept {
    delete _mn;
    if (_is_graphical)
        _mn = new Menu::ErrorMenuSFML(str_error);
    else
        _mn = new Menu::ErrorMenuNcurses(str_error);
}