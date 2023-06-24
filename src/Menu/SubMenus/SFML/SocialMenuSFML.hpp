#ifndef LTYPE_MENU_SOCIALMENUSFML_H
#define LTYPE_MENU_SOCIALMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../SocialMenu.hpp"
#include <random>
#include <string>

namespace Menu {
    class SocialMenuSFML : public BaseMenuSFML, public SocialMenu {
        sf::View _view_scroll{};
    public:
        SocialMenuSFML() noexcept;

        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME windowInputs(sf::Event &event) noexcept override;

        void animation() noexcept override;
        void createButtons() noexcept;

        void setFriendsList(const char* fl, unsigned count_friends) noexcept override;
        void setSearchText() override;

        ~SocialMenuSFML() noexcept override =default;
    private:
        void scrollBtn(float scroll) noexcept;
        void searchFriend() override;
        MNAME goTo() noexcept;
    };
}

#endif
