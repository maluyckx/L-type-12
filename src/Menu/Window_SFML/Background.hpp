#ifndef LTYPE_BackgroundSFML_H
#define LTYPE_BackgroundSFML_H

#include <SFML/Graphics.hpp>
#include <string>

namespace SFML {

        /*
         * Simplifications of the background, and a super simple use in SFML
        */
        class Background {        
                sf::Texture back_texture{};
                sf::Sprite back_sprite1{};
                sf::Sprite back_sprite2{};
                float bg1_y{};
                float bg2_y{};

        public:
                Background() noexcept = default;
                explicit Background(std::string path) noexcept;
                void update(float speed);
                void draw(sf::RenderWindow &window);
        
                ~Background() noexcept = default;
        };
}


#endif