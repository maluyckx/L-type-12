#ifndef LTYPE_MENU_MYPROFILEMENUSFML_H
#define LTYPE_MENU_MYPROFILEMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../MyProfileMenu.hpp"
#include <string>


namespace Menu {
    class MyProfileMenuSFML : public BaseMenuSFML, public MyProfileMenu {
        float _scrolling = 0.f;
        std::vector<std::string> _friends_waiting{};
        sf::View _view_scroll{};
    public:
        MyProfileMenuSFML() noexcept;
        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        void animation() noexcept override;
        void scrollBtn(float scroll) noexcept;

        // REQUEST and Answer
        void setUser(int score, char* question, char* answer, const char* pending_list) override;

        ~MyProfileMenuSFML() noexcept override = default;
    private:
        void setPlanet() noexcept;
        // Draw system
        void creatTexte() noexcept;
        void createButtons() noexcept;
        MNAME goTo() noexcept;
        // Change info$
        void changeInfo();
    };
}


#endif
