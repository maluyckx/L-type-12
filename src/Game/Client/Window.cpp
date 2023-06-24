#include "Window.hpp"
#include "GameClient.hpp"

Game::Client::Window::Window(Game::Client::GameClient* client,
                             Game::Client::DisplayMode mode,
                             Menu::WindowMenu* menu_window,
                             GameplayWindow* game_window) noexcept:
    _current_state(WindowState::InMenu),
    _game_client(client), _mode(mode), _menu_window(menu_window),
    _game_window(game_window) {}

Game::Client::Window::Window(Game::Client::Window&& other) noexcept:
    _current_state(other._current_state),
    _game_client(other._game_client),
    _mode(other._mode),
    _menu_window(other._menu_window),
    _game_window(other._game_window) {
    other._game_client = nullptr;
    other._menu_window = nullptr;
    other._game_window = nullptr;
}

Game::Client::Window&
Game::Client::Window::operator=(Game::Client::Window&& rhs) noexcept {
    _current_state = rhs._current_state;
    _game_client   = rhs._game_client;
    _mode          = rhs._mode;
    _menu_window   = rhs._menu_window;
    _game_window   = rhs._game_window;
    rhs._game_client = nullptr;
    rhs._menu_window = nullptr;
    rhs._game_window = nullptr;
    return *this;
}

Game::Client::Window::~Window() noexcept {
    delete _menu_window;
    delete _game_window;
    delete _game_client;
}

void Game::Client::Window::input() {
    if (getState() == WindowState::InMenu) {
        _menu_window->input();
    } else if (hasFocus()) {
        char inputs[INPUT_MAX_SIZE];
        _game_window->input(inputs);
        if (inputs[0] != '\0') {
            getConnection()->getSendingCommandQueue()->push(
                CommandFactory::input(inputs)
            );
        }
    }
}
