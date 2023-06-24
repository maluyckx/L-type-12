#include "GameplayWindow.hpp"
#include "../HUD.hpp"
#include "Window.hpp"

Game::Client::GameplayWindow::GameplayWindow(Window* window) :
    _window(window) {}

void Game::Client::GameplayWindow::init() {
    if (_window->isDisplayTerminal()) {
        // TODO: do we really need all that?
        initscr();            // Start Ncurses
        cbreak();             // Keyboard input
        noecho();             // Disable the display of entered characters
        keypad(stdscr, TRUE); // Activate specific keys
        resizeterm(42, 126); // Ncurses info
        timeout(0);
        refresh();            // Update the display
        curs_set(FALSE);
        clear();
        endwin();
    }
}

void Game::Client::GameplayWindow::display(sf::RenderWindow* window) {
    for (auto drawable: _background_layer) {
        drawable->draw(window);
    }
    for (auto drawable: _middleground_layer) {
        drawable->draw(window);
    }
    for (auto drawable: _foreground_layer) {
        drawable->draw(window);
    }
}

void Game::Client::GameplayWindow::input(char* out) {
    int i  = 0;
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        out[i++] = static_cast<char>(GAME_KEYS::UP_P2);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        out[i++] = static_cast<char>(GAME_KEYS::DOWN_P2);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        out[i++] = static_cast<char>(GAME_KEYS::LEFT_P2);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        out[i++] = static_cast<char>(GAME_KEYS::RIGHT_P2);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        out[i++] = static_cast<char>(GAME_KEYS::SHOOT_P2);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        out[i++] = static_cast<char>(GAME_KEYS::UP_P1);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        out[i++] = static_cast<char>(GAME_KEYS::DOWN_P1);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        out[i++] = static_cast<char>(GAME_KEYS::LEFT_P1);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        out[i++] = static_cast<char>(GAME_KEYS::RIGHT_P1);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        out[i++] = static_cast<char>(GAME_KEYS::SHOOT_P1);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        out[i++] = static_cast<char>(GAME_KEYS::PAUSE);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        out[i++] = static_cast<char>(GAME_KEYS::RESUME);
    }
    if ((i < INPUT_MAX_SIZE - 1) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
        out[i++] = static_cast<char>(GAME_KEYS::QUIT);
    }
    out[i] = '\0';
}

void Game::Client::GameplayWindow::addDrawable(Game::GameEntity* entity) {
    Drawable* drawable_entity = new Drawable(entity);
    if (entity->getType() == EntityType::HUD)
        addDrawable(DrawingLayer::Foreground, drawable_entity);
    else if (entity->getType() == EntityType::Star)
        addDrawable(DrawingLayer::Background, drawable_entity);
    else addDrawable(DrawingLayer::Middleground, drawable_entity);
}

void Game::Client::GameplayWindow::addDrawable(
    Game::Client::GameplayWindow::DrawingLayer layer,
    Game::Client::Drawable* drawable) {
    switch (layer) {
        case DrawingLayer::Background: {
            _background_layer.push_back(drawable);
            break;
        }
        case DrawingLayer::Middleground: {
            _middleground_layer.push_back(drawable);
            break;
        }
        case DrawingLayer::Foreground: {
            _foreground_layer.push_back(drawable);
            break;
        }
    }
}

void Game::Client::GameplayWindow::removeDrawable(unsigned long id) {
    for (std::size_t i = 0; i < _middleground_layer.size(); i++) {
        if (_middleground_layer[i]->getEntity().getId() == id) {
            removeDrawable(DrawingLayer::Middleground,
                           static_cast<ptrdiff_t>(i));
            return;
        }
    }
    for (std::size_t i = 0; i < _foreground_layer.size(); i++) {
        if (_foreground_layer[i]->getEntity().getId() == id) {
            removeDrawable(DrawingLayer::Foreground, static_cast<ptrdiff_t>(i));
            return;
        }
    }
    for (std::size_t i = 0; i < _background_layer.size(); i++) {
        if (_background_layer[i]->getEntity().getId() == id) {
            removeDrawable(DrawingLayer::Background, static_cast<ptrdiff_t>(i));
            return;
        }
    }
}

void Game::Client::GameplayWindow::removeDrawable(
    Game::Client::GameplayWindow::DrawingLayer layer, ptrdiff_t i) {
    switch (layer) {
        case DrawingLayer::Background: {
            delete _background_layer[static_cast<unsigned long>(i)];
            _background_layer.erase(_background_layer.begin() + i);
            break;
        }
        case DrawingLayer::Middleground: {
            delete _middleground_layer[static_cast<unsigned long>(i)];
            _middleground_layer.erase(_middleground_layer.begin() + i);
            break;
        }
        case DrawingLayer::Foreground: {
            delete _foreground_layer[static_cast<unsigned long>(i)];
            _foreground_layer.erase(_foreground_layer.begin() + i);
            break;
        }
    }
}

void Game::Client::GameplayWindow::clearLayer(
    Game::Client::GameplayWindow::DrawingLayer layer) {
    switch (layer) {
        case DrawingLayer::Background: {
            for (auto& drawable: _background_layer) {
                delete drawable;
            }
            _background_layer.clear();
            break;
        }
        case DrawingLayer::Middleground: {
            for (auto& drawable: _middleground_layer) {
                delete drawable;
            }
            _middleground_layer.clear();
            break;
        }
        case DrawingLayer::Foreground: {
            for (auto& drawable: _foreground_layer) {
                delete drawable;
            }
            _foreground_layer.clear();
            break;
        }
    }
}

void Game::Client::GameplayWindow::clearAllLayers() {
    clearLayer(DrawingLayer::Background);
    clearLayer(DrawingLayer::Middleground);
    clearLayer(DrawingLayer::Foreground);
}
