#include "Stars.hpp"

SFML::Stars::Stars(std::string path) noexcept {
    for (int i = 0; i < 2; ++i) {
        _stars[i] = new SFML::Background(path+std::to_string(i));
    }
}

void SFML::Stars::update(float speed) {
    for (int i = 0; i < 2; ++i) {
        _stars[i]->update(speed/(static_cast<float>(i + 1)));
    }
}

void SFML::Stars::draw(sf::RenderWindow &window) {
    for (auto & _star : _stars) {
        _star->draw(window);
    }
}