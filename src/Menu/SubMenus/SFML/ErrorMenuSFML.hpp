#ifndef LTYPE_MENU_ERRORMENUSFML_H
#define LTYPE_MENU_ERRORMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include <string>


namespace Menu {

    class ErrorMenuSFML : public BaseMenuSFML {
        char _str_error[64]{};
    public:
        ErrorMenuSFML() noexcept = default;
        explicit ErrorMenuSFML(char* str_error) noexcept;

        void initAll() noexcept override;
        MNAME windowInputs(sf::Event&) noexcept override;

        ~ErrorMenuSFML() noexcept override =default;
    };
}

#endif