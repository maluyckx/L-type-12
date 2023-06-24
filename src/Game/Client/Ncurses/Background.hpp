#ifndef LTYPE_GAME_BACKGROUND_H
#define LTYPE_GAME_BACKGROUND_H

#include <array>
#include <cstddef>
#include <SFML/Graphics.hpp>
#include "../../../Utils/Time.hpp"

namespace Ncurses {

    enum STARS {
        BACK = 5, MIDDLE = 4, FRONT = 3
    };

    /// Background with parralax for ncurses
    class Background {
        std::array<
            std::array<int, 2>,
            STARS::BACK + STARS::MIDDLE + STARS::FRONT
        >            _stars{};
        int          timer        = 0;
        Utils::Timer _frame_timer = Utils::Timer(0.033); // 33ms = 30FPS
    public:
        Background() noexcept;
        /// Draws the Stars
        void display() noexcept;
        /// Move all the stars
        void move() noexcept;
        ~Background() noexcept = default;

    private:
        void clearStars() noexcept;
        /// Moves individual stars
        void moveStar(size_t who) noexcept;
        /// Move start from bottom border back to top
        void respawnStar(size_t who) noexcept;
    };

}

#endif
