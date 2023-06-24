#ifndef LTYPE_MENU_USERNAMEVERIFICATIONMENUNCURSES_H
#define LTYPE_MENU_USERNAMEVERIFICATIONMENUNCURSES_H

#include "ConnectionInfoNcurses.hpp"

namespace Menu {
    class UserNameVerificationMenuNcurses : public ConnectionInfoNcurses {
    public:
        UserNameVerificationMenuNcurses() noexcept;
        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;

        void setTextError() noexcept override;
        
        ~UserNameVerificationMenuNcurses() noexcept override =default;
    private:
        // Test username
        MNAME testUser() noexcept;
    };
}


#endif
