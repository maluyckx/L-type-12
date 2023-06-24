#ifndef LTYPE_MENU_CONNECTIONINFONCURSES_H
#define LTYPE_MENU_CONNECTIONINFONCURSES_H

#include "../ConnectionInfo.hpp"
#include "../../../BaseMenuNcurses.hpp"
#include <string>

namespace Menu {

    class ConnectionInfoNcurses : public ConnectionInfo, public BaseMenuNcurses {
    protected:
        ConnectionInfoNcurses() noexcept = default;
    public:
        bool testValues() noexcept override;

        ~ConnectionInfoNcurses() noexcept override =default;
    };
}

#endif
