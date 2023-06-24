#include "GraphicalWindow.hpp"


Game::Client::GraphicalWindow::
GraphicalWindow(Game::Client::GameClient* client) :
    Window(client,
           DisplayMode::Graphical,
           new Menu::WindowMenu(
               client,
               _sfml_window = new sf::RenderWindow(
                   sf::VideoMode(
                       static_cast<unsigned>(WIDTH_WINDOW_SFML),
                       static_cast<unsigned>(HEIGHT_WINDOW_SFML)
                   ),
                   "L-TYPE-12",
                   sf::Style::Close | sf::Style::Titlebar)
           ),
           new GameplayWindow(this)) {
    if (_icon.loadFromFile("sprites/titles/icon.png"))
        _sfml_window->setIcon(64, 64, _icon.getPixelsPtr());
    _menu_window->init();
    _game_window->init();
}

Game::Client::GraphicalWindow::GraphicalWindow(
    Game::Client::GraphicalWindow&& other) noexcept:
    Window(std::move(other)), _background(other._background),
    _stars(other._stars) {

}

Game::Client::GraphicalWindow& Game::Client::GraphicalWindow::operator=(
    Game::Client::GraphicalWindow&& rhs) noexcept {
    Window::operator=(std::move(rhs));
    _background = rhs._background;
    _stars      = rhs._stars;
    rhs._background = nullptr;
    rhs._stars      = nullptr;
    return *this;
}

Game::Client::GraphicalWindow::~GraphicalWindow() noexcept {
    delete _game_window;
    delete _menu_window;
    delete _background;
    delete _stars;
}

void Game::Client::GraphicalWindow::display() {
    if (getState() == WindowState::InMenu) {
        _sfml_window->clear();
        _menu_window->display();
        _sfml_window->display();
    } else {
        if (auto game_state = _game_client->getGameState()) {
            if (!game_state->isPaused()) {
                game_state->updateGame();
                _background->update(0.1f);
                if (_timer.isTriggered(10 / 1000.)) {
                    _particle->update(0.33f);
                }
            }
        }
        _sfml_window->clear();
        _background->draw(*_sfml_window);
        _stars->draw(*_sfml_window);
        _game_window->display(_sfml_window);
        _particle->display(_sfml_window);
        _sfml_window->display();
    }
}

void Game::Client::GraphicalWindow::clear() {
    _sfml_window->clear();
}

bool Game::Client::GraphicalWindow::isRunning() {
    sf::Event event;
    while (_sfml_window->isOpen() && _sfml_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            return false;
    }
    bool running;
    if (getState() == WindowState::InMenu) {
        running = _menu_window->getState();
    } else {
        running = _game_window->getState();
    }
    return running;
}

bool Game::Client::GraphicalWindow::hasFocus() {
    return _sfml_window->hasFocus();
}
