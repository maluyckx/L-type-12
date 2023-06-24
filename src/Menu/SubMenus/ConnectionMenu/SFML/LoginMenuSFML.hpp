#ifndef LTYPE_MENU_LOGINMENUSFML_H
#define LTYPE_MENU_LOGINMENUSFML_H

#include "ConnectionInfoSFML.hpp"

namespace Menu {

    class LoginMenuSFML : public ConnectionInfoSFML {
    public:
        LoginMenuSFML() noexcept;
        // Basics
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        
        // Aswer server error
        void setTextError() noexcept override;
        
        ~LoginMenuSFML() noexcept override =default;
    private:
        // Login
        void login();
    };
}

#endif
