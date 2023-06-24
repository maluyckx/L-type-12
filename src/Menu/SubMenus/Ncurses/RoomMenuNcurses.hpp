#ifndef LTYPE_MENU_ROOMMENUNCURSES_H
#define LTYPE_MENU_ROOMMENUNCURSES_H

#include "../../BaseMenuNcurses.hpp"
#include "../RoomMenu.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>

namespace Menu {
    class RoomMenuNcurses : public BaseMenuNcurses, public RoomMenu {
    private:
        bool  _choice_array[15] = {false}; //Boolean array to know what _choice has been made
    public:
        RoomMenuNcurses() noexcept;

        void initAll() noexcept override;
        void display() override;
        MNAME inputs() override;
        void setTextError(bool work) noexcept;
        
        ~RoomMenuNcurses() noexcept override = default;
    private:
        // RoomMenu choice logic
        void createButton();
        void createText();
        void decorate() noexcept override;
        void newHighlight(int first,int last);
        void startingHighlight() noexcept;
        void setSettings() noexcept;
    };
}


#endif
