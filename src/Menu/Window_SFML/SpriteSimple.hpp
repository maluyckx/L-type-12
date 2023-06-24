#ifndef LTYPE_SPRITESIMPLE_H
#define LTYPE_SPRITESIMPLE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace SFML {

    /**
     * Simple Sprite display in SFMl with animationsand and everything 
     * SUPER MEGA COOL NO ?
    */
    class SpriteSimple {        
        std::vector<sf::Texture*> _textures{};
        sf::Sprite _sprite{};
        float _animation=0;
        size_t _max{};
        bool _one=false;

    public:
        SpriteSimple() noexcept = default;
        SpriteSimple(const SpriteSimple& b) noexcept = default;
        SpriteSimple& operator=(const SpriteSimple&) noexcept = default;
        SpriteSimple(float x, float y, float w, float h, std::string path) noexcept;
        SpriteSimple(float x, float y, float w, float h, std::string path, size_t nb) noexcept;
        SpriteSimple(float x, float y, float w, float h, std::string path, size_t max_width, bool one_texture) noexcept;

        void move(float x, float y);
        void setPosition(float x, float y);
        void setSize(float w, float h) { _sprite.setScale(w / static_cast<float>(_textures[0]->getSize().x), h /
                                                                                                             static_cast<float>(_textures[0]->getSize().y)); }
        void rotate(float degre) noexcept;
        float getRotation() noexcept { return _sprite.getRotation();}
        void setColor(const sf::Uint8* colors) { _sprite.setColor(sf::Color(colors[0], colors[1], colors[2])); }
        void draw(sf::RenderWindow &window);
        void animate(float advance) noexcept;
        void setFirstFrame() noexcept {_animation=0;}
        bool isLastFrame() const noexcept {return static_cast<size_t>(_animation) >= _max - 1;}
        bool isOutScreen() noexcept;

        ~SpriteSimple() noexcept;
    };
}



#endif