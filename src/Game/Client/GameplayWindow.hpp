#ifndef LTYPE_GAME_GAMEPLAYWINDOW_H
#define LTYPE_GAME_GAMEPLAYWINDOW_H

#include "Drawable.hpp"
#include <vector>
#include "../../Menu/Window/Win.hpp"
#include <SFML/Graphics.hpp>

namespace Game::Client {

    class Window;

    /**
     * Everything that is related to drawing stuff when we are in a game
     */
    class GameplayWindow {
    public:
        enum class DrawingLayer {
            Background,
            Middleground,
            Foreground
        };
    private:
        bool _state = true;
        Window* _window;
        std::vector<Drawable*> _background_layer{};
        std::vector<Drawable*> _middleground_layer{};
        std::vector<Drawable*> _foreground_layer{};
    public:
        GameplayWindow(Window* window);
        GameplayWindow(const GameplayWindow&) noexcept = delete;
        GameplayWindow& operator=(const GameplayWindow&) noexcept = delete;
        void init();
        void input(char* out);
        void display(sf::RenderWindow* window = nullptr);

        bool getState() { return _state; }

        void addDrawable(GameEntity* entity);
        void addDrawable(DrawingLayer layer, Drawable* drawable);
        void removeDrawable(unsigned long id);
        void removeDrawable(DrawingLayer layer, ptrdiff_t i);
        void clearLayer(DrawingLayer layer);
        void clearAllLayers();
    };
}

#endif // LTYPE_GAME_GAMEPLAYWINDOW_H
