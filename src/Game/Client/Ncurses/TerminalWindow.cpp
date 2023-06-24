#include "TerminalWindow.hpp"
#include "../../../Utils/DebugLogger.hpp"

void handle_sinwinch(int) {
    if (system("printf '\x1B[8;42;126t'")) {}
    resizeterm(42, 126); // Ncurses info
}

Game::Client::TerminalWindow::TerminalWindow(Game::Client::GameClient* client) :
    Window(client,
           DisplayMode::Terminal,
           new Menu::WindowMenu(client),
           new GameplayWindow(this)) {
    signal(SIGWINCH, handle_sinwinch);
    if (system("printf '\x1B[8;42;126t'")){} // Set window size
    _menu_window->init();
    _game_window->init();
}

Game::Client::TerminalWindow::TerminalWindow(
    Game::Client::TerminalWindow&& other) noexcept:
    Window(std::move(other)),
    _background(std::move(other._background)),
    _chrono(std::move(other._chrono)) {}

Game::Client::TerminalWindow& Game::Client::TerminalWindow::operator=(
    Game::Client::TerminalWindow&& rhs) noexcept {
    Window::operator=(std::move(rhs));
    _background = rhs._background;
    _chrono     = rhs._chrono;
    return *this;
}

Game::Client::TerminalWindow::~TerminalWindow() noexcept {
    delete _game_window;
    delete _menu_window;
}

void Game::Client::TerminalWindow::display() {
    if (getState() == WindowState::InMenu) {
        _menu_window->display();
    } else {
        if (auto game_state = _game_client->getGameState()) {
            if (!game_state->isPaused()) {
                game_state->updateGame();
                _background.move();
            }
        }
        clear();
        _background.display();
        _game_window->display();
        refresh();
    }
}

void Game::Client::TerminalWindow::clear() {
    erase();
}

bool Game::Client::TerminalWindow::isRunning() {
    bool running;
    if (getState() == WindowState::InMenu) {
        running = _menu_window->getState();
    } else {
        running = _game_window->getState();
    }
    return running;
}

bool Game::Client::TerminalWindow::hasFocus() {
    // a bit of a hack, it is impossible to get NCurse window focus, one need
    // to query the OS for that, which depends on the Desktop environement,
    // a PIA to deal with and way out of my competencies.
    // The solution: return true if we pressed a key in the last 0.5s, seems
    // to work
    if(getch() != EOF) {
        _chrono.start();
        return true;
    }
    _chrono.stop();
    return _chrono.getElapsedSecs() <= 0.5;
}


