#ifndef LTYPE_GAME_DRAWABLE_H
#define LTYPE_GAME_DRAWABLE_H

#include <string>
#include <ncurses.h>
#include "../Abstract/GameEntity.hpp"
#include <SFML/Graphics.hpp>

namespace Game::Client {
    /**
     * Drawable is "something" that can be drawn on the screen.
     * It is created with an instance of an entity,
     * so we have all data necessary to draw (hopefully)
     * Why? Decoupling of the entity and the drawing
     */
    class Drawable {
    private:
        GameEntity* _entity;
    public:
        Drawable() = delete;
        Drawable(Game::GameEntity* entity) noexcept;
        GameEntity& getEntity() { return *_entity; }
        void draw(sf::RenderWindow* window = nullptr);

        ~Drawable() noexcept = default;
    };
}

#endif
