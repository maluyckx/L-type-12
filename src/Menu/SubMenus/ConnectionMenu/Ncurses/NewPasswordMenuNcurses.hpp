#ifndef LTYPE_MENU_NEWPASSWORDMENUNCURSES_H
#define LTYPE_MENU_NEWPASSWORDMENUNCURSES_H

#include "ConnectionInfoNcurses.hpp"


namespace Menu {

    class NewPasswordMenuNcurses : public ConnectionInfoNcurses {
    public:
        NewPasswordMenuNcurses() noexcept;

        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;
 
        void setTextError() noexcept override;

        ~NewPasswordMenuNcurses() noexcept override =default;
    private:
        // Set password
        bool testPassword() noexcept;
        MNAME setPassword() noexcept;     
    };
}

#endif
