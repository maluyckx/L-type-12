#ifndef LTYPE_MENU_REGISTERMENUNCURSES_H
#define LTYPE_MENU_REGISTERMENUNCURSES_H

#include "ConnectionInfoNcurses.hpp"

namespace Menu {

    class RegisterMenuNcurses : public ConnectionInfoNcurses {
    public:
        RegisterMenuNcurses() noexcept;
        // Basics
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;

        // Server error answer
        void setTextError() noexcept override;

        ~RegisterMenuNcurses() noexcept override =default;
    private:
        // Create account answer serv
        void createAccount();
    };
}

#endif
