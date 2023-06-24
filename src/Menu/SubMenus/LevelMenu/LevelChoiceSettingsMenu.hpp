#ifndef LTYPE_MENU_LEVELCHOICESETTINGSMENUSFML_H
#define LTYPE_MENU_LEVELCHOICESETTINGSMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../ConnectionMenu/ConnectionInfo.hpp"

namespace Menu {

    class LevelChoiceSettingsMenu : public BaseMenuSFML, public ConnectionInfo {
        int _choosen_time = -1;
    protected:
        static int  _timer_level;
        static char _name_level[LEVEL_NAME_MAX_SIZE];
    public:
        LevelChoiceSettingsMenu() noexcept;

        // BASICS
        void initAll() noexcept override;
        MNAME windowInputs(sf::Event &event) noexcept override;

        void animation() noexcept override;

        void setTextError() noexcept override;
        bool testValues() noexcept override;

        static int getTimeLevel() { return _timer_level; }
        static void setTimeLevel (int time) { _timer_level = time; }
        static auto getNameLevel() { return _name_level; }

        ~LevelChoiceSettingsMenu() noexcept override {};
    };
}

#endif // LTYPE_MENU_LEVELCHOICESETTINGSMENUSFML_H