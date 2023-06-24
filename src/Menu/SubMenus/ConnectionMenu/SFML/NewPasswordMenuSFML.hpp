#ifndef LTYPE_MENU_NEWPASSWORDMENUSFML_H
#define LTYPE_MENU_NEWPASSWORDMENUSFML_H

#include "ConnectionInfoSFML.hpp"


namespace Menu {

    class NewPasswordMenuSFML : public ConnectionInfoSFML {
    public:
        NewPasswordMenuSFML() noexcept;

        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;
 
        void setTextError() noexcept override;

        ~NewPasswordMenuSFML() noexcept override =default;
    private:
        // Set password
        bool testPassword() noexcept;
        MNAME setPassword() noexcept;     
    };
}

#endif
