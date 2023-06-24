#ifndef LTYPE_MENU_LEVELFRIENDMENU_H
#define LTYPE_MENU_LEVELFRIENDMENU_H

#include "../../BaseMenuSFML.hpp"
#include "../FriendProfileMenu.hpp"
#include "../../MenuInfo.hpp"

namespace Menu {

    class LevelFriendMenuSFML : public BaseMenuSFML, protected MenuInfo{
    private:
        std::vector<std::string> _list_levels{};
        std::vector<std::string> _name_levels{};
        sf::View _view_scroll{};
        std::size_t _scrolling = 0;
        std::string _friend{};
    public:
        LevelFriendMenuSFML() noexcept;
        // BASICS
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        void display() override;
        void animation() noexcept override;
        
        void scrollBtn(float scroll) noexcept;
        void createButtons() noexcept;
        void getLevels();
        void setLevelList(const char* levels, int* level_like, unsigned nb_levels) noexcept;

        //void setFriend(char* friend) noexcept;

        ~LevelFriendMenuSFML() noexcept override = default;
    private:
    };
}


#endif
