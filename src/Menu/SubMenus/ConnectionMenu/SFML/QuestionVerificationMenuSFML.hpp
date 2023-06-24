#ifndef LTYPE_MENU_QUESTIONVERIFICATIONMENUSFML_H
#define LTYPE_MENU_QUESTIONVERIFICATIONMENUSFML_H

#include "ConnectionInfoSFML.hpp"

namespace Menu {

    class QuestionVerificationMenuSFML : public ConnectionInfoSFML {
        char _question[64]{};
    public:
        explicit QuestionVerificationMenuSFML(char* question_str) noexcept;
        // BASICS
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        // Server error answer
        void setTextError() noexcept override;

        ~QuestionVerificationMenuSFML()noexcept override =default;
    private:
        // User test
        MNAME testQuestion() noexcept;
    };
}

#endif
