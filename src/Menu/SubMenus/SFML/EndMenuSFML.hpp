#ifndef LTYPE_MENU_ENDMENUSFML_H
#define LTYPE_MENU_ENDMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../EndMenu.hpp"


namespace Menu {

    class EndMenuSFML : public BaseMenuSFML, public EndMenu {
        bool _animation_end=true;
        size_t _animation_timer=0;
    public:
        EndMenuSFML() noexcept = default;
        EndMenuSFML(int level_remain, int score_user, int score_friend) noexcept;

        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        void animation() noexcept override;
        void setButtonLike (bool already_liked) override;
        ~EndMenuSFML() noexcept override =default;

    private:
        void looseAnimation() noexcept;
        void winAnimation() noexcept;    
    };
}

#endif