#ifndef LTYPE_STARS_H
#define LTYPE_STARS_H

#include "Background.hpp"

namespace SFML {

        /**
         * Simple stars from the background, use two background for display them
         * PARRALAX STARS 
         */
        class Stars {        
                Background* _stars[2]{};
        public:
                Stars() noexcept = default;
                Stars(const Stars& b) noexcept = default;
                Stars& operator=(const Stars&) noexcept = default;
                explicit Stars(std::string path) noexcept;
                void update(float speed);
                void draw(sf::RenderWindow &window);

                ~Stars() noexcept = default;
        };      
}



#endif