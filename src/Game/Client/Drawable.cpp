#include "Drawable.hpp"


Game::Client::Drawable::Drawable(Game::GameEntity* entity) noexcept:
        _entity(entity) {}

void Game::Client::Drawable::draw(sf::RenderWindow* window) {
    _entity->draw(window);
}
