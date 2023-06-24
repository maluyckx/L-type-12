#ifndef LTYPE_MENU_WINDOWMENU_H
#define LTYPE_MENU_WINDOWMENU_H

#include "../Game/Client/GameClient.hpp"
#include "BaseMenu.hpp"
#include "BaseMenuSFML.hpp"
#include "MenuInfo.hpp"

#include "SubMenus/RoomMenu.hpp"
#include "SubMenus/LeaderboardMenu.hpp"
#include "SubMenus/SocialMenu.hpp"
#include "SubMenus/MyProfileMenu.hpp"
#include "SubMenus/FriendProfileMenu.hpp"
#include "SubMenus/EndMenu.hpp"
#include "SubMenus/ConnectionMenu/ConnectionInfo.hpp"

#include "SubMenus/Ncurses/MainMenuNcurses.hpp"
#include "SubMenus/Ncurses/RoomMenuNcurses.hpp"
#include "SubMenus/Ncurses/LeaderboardMenuNcurses.hpp"
#include "SubMenus/Ncurses/SocialMenuNcurses.hpp"
#include "SubMenus/Ncurses/FriendProfileMenuNcurses.hpp"
#include "SubMenus/Ncurses/MyProfileMenuNcurses.hpp"
#include "SubMenus/ConnectionMenu/Ncurses/ConnectionMenuNcurses.hpp"
#include "SubMenus/ConnectionMenu/Ncurses/LoginMenuNcurses.hpp"
#include "SubMenus/ConnectionMenu/Ncurses/RegisterMenuNcurses.hpp"
#include "SubMenus/ConnectionMenu/Ncurses/QuestionVerificationMenuNcurses.hpp"
#include "SubMenus/ConnectionMenu/Ncurses/UserNameVerificationMenuNcurses.hpp"
#include "SubMenus/ConnectionMenu/Ncurses/NewPasswordMenuNcurses.hpp"
#include "SubMenus/Ncurses/EndMenuNcurses.hpp"
#include "SubMenus/Ncurses/ErrorMenuNcurses.hpp"
#include "SubMenus/Ncurses/LevelFriendMenuNcurses.hpp"

#include "SubMenus/SFML/MainMenuSFML.hpp"
#include "SubMenus/SFML/RoomMenuSFML.hpp"
#include "SubMenus/SFML/LeaderboardMenuSFML.hpp"
#include "SubMenus/SFML/SocialMenuSFML.hpp"
#include "SubMenus/SFML/FriendProfileMenuSFML.hpp"
#include "SubMenus/SFML/MyProfileMenuSFML.hpp"
#include "SubMenus/ConnectionMenu/SFML/ConnectionMenuSFML.hpp"
#include "SubMenus/ConnectionMenu/SFML/LoginMenuSFML.hpp"
#include "SubMenus/ConnectionMenu/SFML/RegisterMenuSFML.hpp"
#include "SubMenus/ConnectionMenu/SFML/QuestionVerificationMenuSFML.hpp"
#include "SubMenus/ConnectionMenu/SFML/UserNameVerificationMenuSFML.hpp"
#include "SubMenus/ConnectionMenu/SFML/NewPasswordMenuSFML.hpp"
#include "SubMenus/SFML/EndMenuSFML.hpp"
#include "SubMenus/SFML/ErrorMenuSFML.hpp"
#include "SubMenus/SFML/LevelFriendMenuSFML.hpp"

#include "SubMenus/LevelMenu/LevelMainMenu.hpp"
#include "SubMenus/LevelMenu/LevelCreateMenu.hpp"
#include "SubMenus/LevelMenu/LevelChoiceSettingsMenu.hpp"


namespace Menu {
    
    /* The main window menu class who manage all menus, manage commands from 
     * server, this class manages menu navigation, he use abstrac class BaseMenu 
     * who are inherit from all menus. 
    */
    class WindowMenu {
        bool _is_graphical{};
        bool _state = true;
        Menu::BaseMenu* _mn{};
        Game::Client::GameClient* _client{};
        MNAME _menu_type = MNAME::CONNECTION_MENU;

    public:
        constexpr WindowMenu() noexcept = default;
        explicit WindowMenu(Game::Client::GameClient* client) noexcept: _client(client) {_is_graphical=false;} // NCURSES
        constexpr WindowMenu(Game::Client::GameClient* client, sf::RenderWindow* wd) noexcept: _client(client) 
                                {Menu::BaseMenuSFML::setWindow(wd);_is_graphical=true;} // SFML

        // Simple function who initialize the basics variable or function for Menus, and
        // it check if where in graphical or terminal
        void init();

        // Function who display all contents from the menus, user polymorphisme for know who display
        // call
        void display();

        // Manages all inputs from windows, and manage the naviagation between each menu
        void input();

        // get the state of the WindowMenu, if he's activate or not
        bool getState() const noexcept;

        // Activate Menu, use by game client
        inline void activateMenu() noexcept {_state=true;}

        /*** SERVER FUNCTION
         * all return functions from server
        * Use dynamic cast for server command return. 
         * ***/
        // Server know what Menu is, and so can send good information
        MNAME getMenuType() noexcept {return _menu_type;}
        // If register works or not
        void setRegister(bool work) noexcept;
        // If you can login or not
        void setLogin(bool work) noexcept;
        // Set the level Name menu (idk UwU)
        void setLevelChoiceNameMenu(bool work);
        // if the creation of the level works
        void setLevelCreateMenu(bool work) noexcept;
        // Get all menus informations from server
        void friendLevelMenuSetup(char* levels_list, int* levels_like_list, unsigned level_count) noexcept;
        // Get secret question from the user
        void secretQuestion(char* question_str) noexcept;
        // If the answer to the player work
        void secretAnswer(bool work) noexcept;
        // Get all friends list
        void socialMenuSetup(char* friends_list, unsigned count_friends) noexcept;
        // If the search for a friend work
        void findFriend(bool is_existing) noexcept;
        // Get all informations from leaderboard
        void setLeaderboard(char* usernames_list, int* scores_list) noexcept;
        // Get user score
        void setMyLeaderboard(int my_score) noexcept;
        // Get all informations for the user profile menu
        void setProfileUser(int score, char* question, char* answer, char* pending_list) noexcept;
        // Get friend informations, if their friend or not
        void setFriendInfo(FRIEND_STATE state, int score) noexcept;
        // Return from the game to menu and get all informations from the menu
        void setEndMenu(int end_code, int score_user, int score_friend) noexcept;
        // Error menu hehe (not really usefull cause we don't have bugs #swag #thug)
        void setMenuError(char* str_error) noexcept;
        void setLevelRoomMenu(bool work);
        // Get information from the menu, if he's already like or not
        void setButtonLikeEndMenu(bool already_liked);

        // void setLevelFriendMenu(char* friend);
        // void myFriendLevelMenuSetup(char* levels_list, int* levels_like_list, unsigned level_count) noexcept;

        ~WindowMenu() noexcept = default;
        // UwU
    };
}

#endif