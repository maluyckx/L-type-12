#ifndef LTYPE_MENU_QUESTIONVERIFICATIONMENUNCURSES_H
#define LTYPE_MENU_QUESTIONVERIFICATIONMENUNCURSES_H

#include "ConnectionInfoNcurses.hpp"

namespace Menu {

    class QuestionVerificationMenuNcurses : public ConnectionInfoNcurses {
        char _question[64]{};
    public:
        explicit QuestionVerificationMenuNcurses(char* question_str) noexcept;
        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;
        // Server error answer
        void setTextError() noexcept override;

        ~QuestionVerificationMenuNcurses()noexcept override =default;
    private:
        // User test
        MNAME testQuestion() noexcept;
    };
}

#endif
