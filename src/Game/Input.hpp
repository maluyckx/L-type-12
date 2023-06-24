#ifndef LTYPE_GAME_INPUT_H
#define LTYPE_GAME_INPUT_H

#include "../Utils/Constants.hpp"

namespace Game {
    class Input {
    private:
        enum KeyFlags {
            Player1_Up    = 1 << 0,
            Player1_Down  = 1 << 1,
            Player1_Left  = 1 << 2,
            Player1_Right = 1 << 3,
            Player1_Shoot = 1 << 4,

            Player2_Up    = 1 << 5,
            Player2_Down  = 1 << 6,
            Player2_Left  = 1 << 7,
            Player2_Right = 1 << 8,
            Player2_Shoot = 1 << 9,

            Pause         = 1 << 10,
            Resume        = 1 << 11,
            Quit          = 1 << 12,
        };
        unsigned _table;
    public:
        void updateInputTable(const char* inputs);
        void clearInputTable();
        bool isKeyDown(GAME_KEYS key) const;
    };
}

#endif
