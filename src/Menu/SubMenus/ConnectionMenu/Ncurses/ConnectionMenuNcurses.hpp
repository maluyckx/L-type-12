#ifndef LTYPE_MENU_CONNECTIONMENUNCURSES_H
#define LTYPE_MENU_CONNECTIONMENUNCURSES_H

#include "../../../BaseMenuNcurses.hpp"
#include "../../../MenuInfo.hpp"
#include <string>


namespace Menu {

    class ConnectionMenuNcurses : public BaseMenuNcurses, protected MenuInfo {
        int _ship_speed=0;
    public:
        ConnectionMenuNcurses() noexcept {initAll();}
        ConnectionMenuNcurses(Game::Client::GameClient* client) noexcept;

        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;

        ~ConnectionMenuNcurses() noexcept override =default;

    private:
        // decorate
        void decorate() noexcept override;
        void animation() noexcept override;
    };
}

#endif