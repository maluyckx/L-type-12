#ifndef LTYPE_MENU_ENDMENUNCURSES_H
#define LTYPE_MENU_ENDMENUNCURSES_H

#include "../../BaseMenuNcurses.hpp"
#include "../EndMenu.hpp"

namespace Menu {

    class EndMenuNcurses : public BaseMenuNcurses, public EndMenu {
    public:
        EndMenuNcurses() noexcept = default;
        EndMenuNcurses(int level_remain, int score_user, int score_friend) noexcept;

        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;
        void setButtonLike(bool already_liked) override;
        ~EndMenuNcurses() noexcept override =default;
    private:
        // decorate
        void decorate() noexcept override;
        void animation() noexcept override;
    };
}

#endif