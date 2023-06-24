#ifndef LTYPE_MENU_MENUINFO_H
#define LTYPE_MENU_MENUINFO_H

#include "../Game/Client/GameClient.hpp"
#include "../Game/Command.hpp"

namespace Menu {

    /** 
     * Full static protected class, this class have all informations from the client
     * Menu, this class is inherited by all menus.
     * This class is full static cause is usless to delete or set a pointer each time for
     * each menu
     * Informations are initialize from the WindowMenu at the beginning
    */
    class MenuInfo {
    protected: 
        static Game::Client::GameClient* _client;
        static char _user_name[USERNAME_MAX_SIZE];
        static char _friend_name[USERNAME_MAX_SIZE];
        MenuInfo() noexcept = default;
        MenuInfo(const MenuInfo&) noexcept = default;
        MenuInfo& operator=(const MenuInfo&) noexcept = default;
    
        /* GETTER SETTER FULL PROTECTED */
        static void setFriendName(std::string fn) noexcept;
        static void setClient(Game::Client::GameClient* client) noexcept {_client=client;}

        static char* getUserName() noexcept {return _user_name;}
        static char* getFriendName() noexcept {return _friend_name;}
        static Game::Client::GameClient* getClient() noexcept {return _client;}

    public:
        virtual ~MenuInfo() noexcept = default;
    };
}

#endif