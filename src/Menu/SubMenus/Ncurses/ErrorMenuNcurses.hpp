#ifndef LTYPE_MENU_ERRORMENUNCURSES_H
#define LTYPE_MENU_ERRORMENUNCURSES_H

#include "../../BaseMenuNcurses.hpp"
#include <string>


namespace Menu {

    class ErrorMenuNcurses : public BaseMenuNcurses {
        char _str_error[64]{};
    public:
        ErrorMenuNcurses() noexcept = default;
        explicit ErrorMenuNcurses(char* str_error) noexcept;

        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;

        ~ErrorMenuNcurses() noexcept override =default;
    private:
        // decorate
        void decorate() noexcept override;
        void animation() noexcept override;
    };
}

#endif