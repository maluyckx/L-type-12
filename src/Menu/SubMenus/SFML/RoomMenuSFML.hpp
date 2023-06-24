#ifndef LTYPE_MENU_ROOMMENUSFML_H
#define LTYPE_MENU_ROOMMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../RoomMenu.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>

namespace Menu {
    class RoomMenuSFML : public BaseMenuSFML, public RoomMenu {
        
    public:
        RoomMenuSFML() noexcept;

        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        void setTextError(bool work) noexcept;
        void animation() noexcept override;
        ~RoomMenuSFML() noexcept override = default;
    private:
        void startHiglight() noexcept;
        void setSettings() noexcept;
        void newHilight(size_t first, size_t last, size_t who) noexcept;
    };
}


#endif
