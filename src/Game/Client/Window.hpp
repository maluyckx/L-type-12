#ifndef LTYPE_GAME_WINDOW_H
#define LTYPE_GAME_WINDOW_H

#include "../../Menu/WindowMenu.hpp"
#include "Drawable.hpp"
#include "GameplayWindow.hpp"

namespace Game::Client {
    enum class WindowState {
        InMenu,
        InGame,
    };
    enum class DisplayMode {
        Terminal,
        Graphical,
    };

    class GameClient;

    /**
     * Abstraction of a window (either NCurse or SFML)
     */
    class Window {
    protected:
        WindowState _current_state;
        GameClient* _game_client;
        DisplayMode _mode=DisplayMode::Graphical;
        Menu::WindowMenu     * _menu_window;
        GameplayWindow       * _game_window;
    public:
        Window() = delete;
        Window(Game::Client::GameClient* client,
               Game::Client::DisplayMode mode,
               Menu::WindowMenu* menu_window,
               GameplayWindow* game_window) noexcept;
        Window(const Window& other) = delete;
        Window(Window&& other) noexcept;
        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& rhs) noexcept;
        virtual ~Window() noexcept;

        WindowState getState() const {
            return _current_state;
        }

        void setState(WindowState new_state) {
            _current_state = new_state;
        }

        Connection* getConnection() const {
            return _game_client->getConnection();
        }

        DisplayMode getDisplayMode() const {
            return _mode;
        }

        bool isDisplayTerminal() const {
            return _mode == DisplayMode::Terminal;
        }

        bool isDisplayGraphical() const {
            return _mode == DisplayMode::Graphical;
        }

        virtual GameplayWindow* getGameplayWindow() const noexcept = 0;
        virtual bool isRunning() = 0;
        virtual Menu::WindowMenu* getWindowMenu() = 0;
        virtual void input();
        virtual void display() = 0;
        virtual void clear() = 0;
        virtual bool hasFocus() = 0;
    };
}


#endif
