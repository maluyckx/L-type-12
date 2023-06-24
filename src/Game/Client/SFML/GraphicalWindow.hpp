#ifndef LTYPE_GAME_GRAPHICALWINDOW_H
#define LTYPE_GAME_GRAPHICALWINDOW_H

#include "../Window.hpp"
#include "../../../Menu/Window_SFML/Background.hpp"
#include "../../../Menu/Window_SFML/Stars.hpp"
#include "ParticleHandler.hpp"
#include <SFML/Graphics.hpp>

namespace Game::Client {
    /// An instance of a window using SFML
    class GraphicalWindow : public Window {
    private:
        sf::RenderWindow     * _sfml_window;
        sf::Image _icon{};
        SFML::Background     * _background = new SFML::Background("sprites/background/background_1");
        SFML::Stars          * _stars =      new SFML::Stars("sprites/background/stars_");
        SFML::ParticleHandler* _particle =   new SFML::ParticleHandler();
        Utils::Timer _timer = Utils::Timer();

    public:
        GraphicalWindow(GameClient* client);
        GraphicalWindow(const GraphicalWindow& other) = delete;
        GraphicalWindow(GraphicalWindow&& other) noexcept;
        GraphicalWindow& operator=(const GraphicalWindow& rhs) = delete;
        GraphicalWindow& operator=(GraphicalWindow&& rhs) noexcept;
        ~GraphicalWindow() noexcept override;
        void display() override;
        void clear() override;
        bool isRunning() override;
        bool hasFocus() override;

        GameplayWindow* getGameplayWindow() const noexcept override {
            return _game_window;
        }

        Menu::WindowMenu* getWindowMenu() override { return _menu_window; }

        [[maybe_unused]]
        sf::RenderWindow* getWindow() noexcept { return _sfml_window; }

        /// Get the thing that plays the particle
        SFML::ParticleHandler*
        getParticleHandler() noexcept { return _particle; }
    };
}

#endif
