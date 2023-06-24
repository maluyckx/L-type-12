#ifndef LTYPE_MENU_USERNAMEVERIFICATIONMENUSFML_H
#define LTYPE_MENU_USERNAMEVERIFICATIONMENUSFML_H

#include "ConnectionInfoSFML.hpp"

namespace Menu {
    class UserNameVerificationMenuSFML : public ConnectionInfoSFML {
    public:
        UserNameVerificationMenuSFML() noexcept;
        // BASICS
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;

        void setTextError() noexcept override;
        
        ~UserNameVerificationMenuSFML() noexcept override =default;
    private:
        // Test username
        MNAME testUser() noexcept;
    };
}


#endif
