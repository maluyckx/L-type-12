#ifndef LTYPE_MENU_MAINMENUSFML_H
#define LTYPE_MENU_MAINMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../../MenuInfo.hpp"

namespace Menu {

    class MainMenuSFML : public BaseMenuSFML, protected MenuInfo {
    public:
        MainMenuSFML() noexcept;

        // BASICS
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;

        void animation() noexcept override;

        ~MainMenuSFML() noexcept override = default;
    };
}

#endif
