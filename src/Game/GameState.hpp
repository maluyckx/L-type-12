#ifndef LTYPE_GAME_GAMESTATE_H
#define LTYPE_GAME_GAMESTATE_H

// FIXME: Ne fonctionne plus si hpp inclu . Pourquoi ??
#include "Server/CollisionResolver.hpp"
#include "Abstract/GameEntity.hpp"
#include "GameSettings.hpp"
#include <vector>
#include "../Utils/Constants.hpp"
#include "PlayerShip.hpp"
#include "HUD.hpp"
#include "Abstract/Boss.hpp"

namespace Game {
    /**
     * A representation of the state of the game, like a list of all the entities currently present on the screen,
     * wether the game is over or not, if it is paused or not and other "game" states (not settings)
     * 
     * In _all_entities: playership 1 is at index 0 and 2 (if exist) at index 1
     */
    class GameState {
    public:
        enum class State {
            Paused,
            Started,
            Over
        };
    private:
        std::vector<GameEntity*> _all_entities;
        State                    _state;
        std::size_t              _next_id{};
        GameSettings             _settings{};
        char                     _username_player1[USERNAME_MAX_SIZE]{};
        char                     _username_player2[USERNAME_MAX_SIZE]{};
    public:
        GameState() noexcept = default;
        GameState(char* username_player1, char* username_player2,
                  GameSettings settings);
        void addEntity(GameEntity*);
        void deleteEntity(unsigned long id);

        size_t size() { return _all_entities.size(); }

        GameEntity* operator[](std::size_t i) { return _all_entities[i]; }

        GameSettings getGameSettings() { return _settings; }

        std::vector<GameEntity*>& getAllEntities() { return _all_entities; }

        void updateGame(GameInstance* instance = nullptr);
        void updateEntityServerPosition(unsigned long id, Vector2D position);
        void updateHUD(char* username_player1, char* username_player2,
                       int player1_hp, int player1_score,
                       int player2_hp, int player2_score, int boss_hp,
                       int player_max_hp, int boss_max_hp);
        void updateHUDState();

        inline void setState(State state) { _state = state; }

        inline void setSettings(GameSettings settings) { _settings = settings; }

        inline constexpr bool isOver() { return _state == State::Over; }

        inline constexpr bool isPaused() { return _state == State::Paused; }

        inline constexpr bool isStarted() { return _state == State::Started; }
        
        State getState() {
            return _state;
        }

        size_t getNewId() {
            return _next_id++;
        }

        const char* getUsernamePlayer1() const {
            return _username_player1;
        }

        const char* getUsernamePlayer2() const {
            return _username_player2;
        }
        PlayerShip* getPlayerShip(bool first_player);
        HUD* getHUD();
        Boss* getBoss();

        ~GameState() noexcept;
    };
}

#endif
