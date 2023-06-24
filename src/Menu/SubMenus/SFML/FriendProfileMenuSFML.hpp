#ifndef LTYPE_MENU_FRIENDPROFILEMENUSFML_H
#define LTYPE_MENU_FRIENDPROFILEMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../FriendProfileMenu.hpp"

namespace Menu {
    class FriendProfileMenuSFML : public BaseMenuSFML, public FriendProfileMenu {
    public:
        FriendProfileMenuSFML() noexcept;
        // BASICS
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        void animation() noexcept override;

        void setFriendInfo(Menu::FRIEND_STATE state, int score) noexcept override;

        ~FriendProfileMenuSFML() noexcept override = default;
    private:
        void setPlanet() noexcept;
        // Friends system
        void sendFriendRequest() override;
        void deleteFriend() override;
        void acceptFriendRequest() override;
        void rejectFriendRequest() override;
    };
}


#endif
