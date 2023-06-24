#ifndef LTYPE_UTILS_COLORSSFML_H
#define LTYPE_UTILS_COLORSSFML_H

#include <SFML/Graphics.hpp>

/// Color constants for SFML
namespace COLORS_SFML {
    [[maybe_unused]] const sf::Uint8 WHITE[3] = {250, 250, 250};
    [[maybe_unused]] const sf::Uint8 BLACK[3] = {0, 0, 0};
    [[maybe_unused]] const sf::Uint8 RED[3] = {213, 0, 0};
    [[maybe_unused]] const sf::Uint8 BLUE[3] = {48, 79, 254};
    [[maybe_unused]] const sf::Uint8 GREEN[3] = {0, 200, 83};
    [[maybe_unused]] const sf::Uint8 YELLOW[3] = {255, 215, 0};
    [[maybe_unused]] const sf::Uint8 ORANGE[3] = {255,127,80};
}

#endif