#ifndef LTYPE_MENU_LEVELSMENUNCURSES_H
#define LTYPE_MENU_LEVELSMENUNCURSES_H

#include "../../BaseMenuNcurses.hpp"
#include "../../MenuInfo.hpp"
#include <random>
#include <string>

namespace Menu {
    class LevelFriendMenuNcurses : public BaseMenuNcurses, protected MenuInfo {
        size_t _scrolling = 0;
        std::vector<std::string> _list_levels{};
        std::vector<std::string> _name_levels{};
    public:
        LevelFriendMenuNcurses() noexcept;

        // BASICS
        void initAll() noexcept override;
        void display() override;
        
        void getLevels();
        void setLevelList(const char* levels, int* level_like, unsigned nb_levels) noexcept;
        MNAME inputs() override;

        ~LevelFriendMenuNcurses() noexcept override =default;
    private:
        // Social
        void createButtons();

        // deco
        void checkArrow();
        void decorate() noexcept override;
        void animation() noexcept override;

        MNAME goTo() noexcept;
    };
}

#endif
