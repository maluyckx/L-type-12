#ifndef LTYPE_MENU_LEVELMAINMENUSFML_H
#define LTYPE_MENU_LEVELMAINMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../../MenuInfo.hpp"

namespace Menu {

    class LevelMainMenu : public BaseMenuSFML, protected MenuInfo {
    public:
        LevelMainMenu() noexcept;

        // BASICS
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;

        void animation() noexcept override;

        ~LevelMainMenu() noexcept override = default;
    };
}

#endif
