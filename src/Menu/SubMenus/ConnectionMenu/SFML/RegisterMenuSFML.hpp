#ifndef LTYPE_MENU_REGISTERMENUSFML_H
#define LTYPE_MENU_REGISTERMENUSFML_H

#include "ConnectionInfoSFML.hpp"

namespace Menu {

    class RegisterMenuSFML : public ConnectionInfoSFML {
    public:
        RegisterMenuSFML() noexcept;
        // Basics
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;

        // Server error answer
        void setTextError() noexcept override;

        ~RegisterMenuSFML() noexcept override =default;
    private:
        // Create account answer serv
        void createAccount();
    };
}

#endif
