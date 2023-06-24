#ifndef LTYPE_GAME_TERMINALWINDOW_H
#define LTYPE_GAME_TERMINALWINDOW_H

#include "../../../Menu/WindowMenu.hpp"
#include "../Window.hpp"
#include "../GameClient.hpp"
#include "../GameplayWindow.hpp"
#include "Background.hpp"
#include <signal.h>

namespace Game::Client {

    /// An instance of the terminal window with NCurses
    class TerminalWindow : public Window {
    private:
        Ncurses::Background _background = Ncurses::Background();
        Utils::StopWatch    _chrono     = Utils::StopWatch();
    public:
        TerminalWindow(GameClient* client);
        TerminalWindow(const TerminalWindow& other) = delete;
        TerminalWindow(TerminalWindow&& other) noexcept;
        TerminalWindow& operator=(const TerminalWindow& rhs) = delete;
        TerminalWindow& operator=(TerminalWindow&& rhs) noexcept;
        ~TerminalWindow() noexcept override;
        void display() override;
        void clear() override;
        bool isRunning() override;
        bool hasFocus() override;

        GameplayWindow* getGameplayWindow() const noexcept override {
            return _game_window;
        }

        Menu::WindowMenu* getWindowMenu() override { return _menu_window; }
    };
}


#endif
