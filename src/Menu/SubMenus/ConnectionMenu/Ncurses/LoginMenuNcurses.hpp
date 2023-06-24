#ifndef LTYPE_MENU_LOGINMENUNCURSES_H
#define LTYPE_MENU_LOGINMENUNCURSES_H

#include "ConnectionInfoNcurses.hpp"

namespace Menu {

    class LoginMenuNcurses : public ConnectionInfoNcurses {
    public:
        LoginMenuNcurses() noexcept;
        // Basics
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;
        
        // Aswer server error
        void setTextError() noexcept override;
        
        ~LoginMenuNcurses() noexcept override =default;
    private:
        // Login
        void login();
    };
}

#endif
