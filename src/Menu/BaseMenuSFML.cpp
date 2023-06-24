#include "BaseMenuSFML.hpp"
#include <cstdlib>
#include <ctime>

sf::RenderWindow* Menu::BaseMenuSFML::_window;
SFML::Background* Menu::BaseMenuSFML::_background;
SFML::Stars* Menu::BaseMenuSFML::_stars;
std::vector<SFML::SpriteSimple*>  Menu::BaseMenuSFML::_bg_sprites;
int Menu::BaseMenuSFML::_random_spawn=120;

void Menu::BaseMenuSFML::display() {  // Basic draw, can override for change overlapping
    timer(30);
    _window->setView(_window->getDefaultView());
    _background->draw(*_window);
    _stars->draw(*_window);
    for (auto & _bg_sprite : _bg_sprites)
        _bg_sprite->draw(*_window);
    for (auto & _sprite : _sprites)
        _sprite->draw(*_window);
    for (auto & _texte : _textes)
        _texte->draw(*_window);
    for (auto & _button : _buttons)
        _button->draw(*_window);
    for (auto & _txt_field : _txt_fields)
        _txt_field->draw(*_window);
}

MNAME Menu::BaseMenuSFML::inputs() {
    sf::Event event{};
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window->close();
            return MNAME::EXIT_MENU;
        }
        MNAME tmp = windowInputs(event);
        if (tmp!=MNAME::NOTHING)
            return tmp;
        // if (event.type==sf::Event::MouseButtonPressed)
        //     explose(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);
    }
    return MNAME::NOTHING;
}

void Menu::BaseMenuSFML::animation() noexcept {
    for (size_t i = 0; i < _bg_sprites.size(); i++) {
        if (_bg_sprites[i]->isOutScreen()) {
            delete _bg_sprites[i];
            _bg_sprites.erase(_bg_sprites.begin() + static_cast<long>(i));
        }
    }
    _random_spawn--;
    if (_random_spawn == 0)
        generateRandom();
    _background->update(0.6f);  // Basics background
    _stars->update(1.f);  // Basics stars
    // if (_sprites.size()>0) {
    //     for (size_t i = _sprites.size()-_explosion; i < _sprites.size(); i++) {
    //         _sprites[i]->animate(1.5f);
    //         if (_sprites[i]->isLastFrame()) {
    //             delete _sprites[i];
    //             _sprites.erase(_sprites.begin()+i);
    //             _explosion--;
    //         }
    //     }
    // }
    for (auto & _bg_sprite : _bg_sprites) {
        _bg_sprite->move(0, -1);
        _bg_sprite->rotate(0.2f);
    }
}

void Menu::BaseMenuSFML::generateRandom() noexcept {
    srand(static_cast<unsigned>(time(nullptr)));
    _random_spawn= (rand() % 400)+ 200;
    float size_obecjts = static_cast<float>(rand()%64)+48.f;
    _bg_sprites.emplace_back(new SFML::SpriteSimple(
                            static_cast<float>(static_cast<unsigned int>(rand()) % static_cast<unsigned>(WIDTH_WINDOW_SFML)),
                            HEIGHT_WINDOW_SFML,
                            size_obecjts, 
                            size_obecjts, 
                            "sprites/background/objects/object_"+std::to_string(rand()%NB_ENTITY_BACKGROUND)));
}

void Menu::BaseMenuSFML::quitMenu() noexcept {
    delete _background;
    delete _stars;
    for (auto & _bg_sprite : _bg_sprites)
        delete _bg_sprite;
    _window->close();
}

Menu::BaseMenuSFML::~BaseMenuSFML() noexcept {
    // Destroy all statics objects, at the end of the Menu
    for (auto & _button : _buttons)
        delete _button;
    for (auto & _txt_field : _txt_fields)
        delete _txt_field;
    for (auto & _texte : _textes)
        delete _texte;
    for (auto & _sprite : _sprites)
        delete _sprite;
    _buttons.clear();
    _txt_fields.clear();
    _textes.clear();
    _sprites.clear();
}