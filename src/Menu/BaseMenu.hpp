#ifndef LTYPE_MENU_BASEMENU_H
#define LTYPE_MENU_BASEMENU_H

#include "../Utils/Time.hpp"

enum class MNAME {
    EXIT_MENU,
    GAME,
    MAIN_MENU,
    ROOM_MENU,
    SOCIAL_MENU,
    LEADERBOARD_MENU,
    MY_PROFILE_MENU,
    PROFILE_FRIENDS,
    CONNECTION_MENU,
    LOGIN_MENU,
    REGISTER_MENU,
    QUESTION_VERIFICATION_MENU,
    USER_NAME_VERIFICATION_MENU,
    NEW_PASSWORD_MENU,
    LEVEL_MAIN_MENU,
    LEVEL_CHOICE_SETTINGS_MENU,
    LEVEL_CREATE_MENU,
    MY_LEVEL_MENU,
    FRIEND_LEVEL_MENU,
    NOTHING
};

namespace Menu {

    /** 
     * Abstract class Basic Menu, herited from BaseMenuSfml and BaseMenuNcurses 
     * is use from WindowMenu for polymorphe calls
    */
    class BaseMenu {
        Utils::Timer _timer = Utils::Timer();
    protected:
        BaseMenu() noexcept = default;
        BaseMenu(const BaseMenu&) noexcept = default;
        BaseMenu& operator=(const BaseMenu&) noexcept = default;
    public:
        // initializes everything, window, buttons, Win and text sprite
        virtual void initAll() noexcept = 0;
        // Display everything each frame
        virtual void display() = 0;
        // Get inputs and return what he need to do
        virtual MNAME inputs() = 0;

        virtual void quitMenu() noexcept = 0;

        // Function usefull for animations, and has ms.
        void timer(int ms) noexcept;
        virtual void animation() noexcept = 0;

        virtual ~BaseMenu() noexcept = default;
    };
}

#endif