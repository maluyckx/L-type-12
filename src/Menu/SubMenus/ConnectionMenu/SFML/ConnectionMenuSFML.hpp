#ifndef LTYPE_MENU_CONNECTIONMENUSFML_H
#define LTYPE_MENU_CONNECTIONMENUSFML_H

#include "../../../BaseMenuSFML.hpp"
#include "../../../MenuInfo.hpp"

namespace Menu {

    class ConnectionMenuSFML : public BaseMenuSFML, protected MenuInfo {
    public:
        ConnectionMenuSFML() noexcept {initAll();}
        explicit ConnectionMenuSFML(Game::Client::GameClient* client) noexcept;

        void initAll() noexcept override;
        void display() override;
        MNAME windowInputs(sf::Event &event) noexcept override;

        void animation() noexcept override; 

        ~ConnectionMenuSFML() noexcept override = default;
    };
}

#endif