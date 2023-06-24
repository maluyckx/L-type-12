#ifndef LTYPE_GAME_HUD_H
#define LTYPE_GAME_HUD_H

#include "Abstract/GameEntity.hpp"
#include "Abstract/Destructable.hpp"
#include <vector>
#include "../Utils/Constants.hpp"
#include "../Menu/Window_SFML/SpriteSimple.hpp"
#include "../Menu/Window_SFML/Texte.hpp"
#include "../Utils/ColorsSFML.hpp"

namespace Game {

    class PlayerShip;

    class GameInstance;

    class HUD final: public GameEntity {
        char _username_player1[USERNAME_MAX_SIZE]{};
        char _username_player2[USERNAME_MAX_SIZE]{};
        int  _player1_hp;
        int  _player2_hp;
        int  _boss_hp;
        int  _boss_hp_max;  // La vie maximale que le boss puisse avoir (constante)
        int  _player_hp_max;  // La vie maximale que le joueur puisse avoir (constante)
        int  _player1_score;
        int  _player2_score;
        bool _isPaused;
		SFML::SpriteSimple _player_hp = SFML::SpriteSimple(1.f, 0.f, (10.f/static_cast<float>(_player_hp_max))*GRAPHIC_X_MULTIPLIER, GRAPHIC_Y_MULTIPLIER, "sprites/hud/hp");
		SFML::SpriteSimple _hp_boss = SFML::SpriteSimple(0.f, 0.f, 40.f/static_cast<float>(_boss_hp_max)*GRAPHIC_X_MULTIPLIER, GRAPHIC_Y_MULTIPLIER, "sprites/hud/boss_hp");
        SFML::SpriteSimple _boss_bar = SFML::SpriteSimple((WIDTH_WINDOW/2-21.5f)*GRAPHIC_X_MULTIPLIER, (HEIGHT_WINDOW - EntityDimensions::HUD_HEIGHT + 0.05f)*GRAPHIC_Y_MULTIPLIER, 31.f*GRAPHIC_X_MULTIPLIER, 2.8f*GRAPHIC_Y_MULTIPLIER, "sprites/hud/boss_bar");
        SFML::SpriteSimple _player2_bar = SFML::SpriteSimple(0.f, (HEIGHT_WINDOW - EntityDimensions::HUD_HEIGHT)*GRAPHIC_Y_MULTIPLIER, EntityDimensions::HUD_WIDTH*GRAPHIC_X_MULTIPLIER, EntityDimensions::HUD_HEIGHT*GRAPHIC_Y_MULTIPLIER, "sprites/hud/player2_bar");
        SFML::Texte _text = SFML::Texte("", 0.f, 0.f, 12.f, COLORS_SFML::YELLOW);
        
    private:
    	void drawLife(int player, int life, int life_max, sf::RenderWindow* window) noexcept;
    	void drawLifeBoss(int life, int life_max, sf::RenderWindow* window) noexcept;
    	void drawScore(int player, int score, sf::RenderWindow* window);
    	
    public:
        HUD() = delete;
        HUD(unsigned long id,
            const char* username_player1,
            const char* username_player2);
        HUD(unsigned long id,
            const char* username_player1,
            const char* username_player2,
            int life_max_player);   

        void updateContent(
            char* username_player1,
            char* username_player2,
            int player1_hp,
            int player2_hp,
            int boss_hp,
            int player1_score,
            int player2_score,
            int player_max_hp,
            int boss_max_hp
        );
        void displayPause(sf::RenderWindow* window);

        void setIsPaused(bool isPaused) { _isPaused = isPaused; }

        void setMaxLifePlayer(int life_max) { _player_hp_max = life_max; }

        const char* getUsernamePlayer1() const {
            return _username_player1;
        }

        const char* getUsernamePlayer2() const {
            return _username_player2;
        }

        int getBossHp() const {
            return _boss_hp;
        }

        int getBossHpMax() const {
            return _boss_hp_max;
        }

        int getPlayer1Hp() const {
            return _player1_hp;
        }

        int getPlayer2Hp() const {
            return _player2_hp;
        }

        int getPlayer1Score() const {
            return _player1_score;
        }

        int getPlayer2Score() const {
            return _player2_score;
        }

        int getPlayerHpMax() const {
            return _player_hp_max;
        }
        
        void setBossMaxHp(int max_hp) {
        	_boss_hp_max = max_hp;
        }

        void onUpdate(GameInstance* instance = nullptr) override;
        
        void onCollision(Game::GameEntity*,
                         GameInstance*) override {};

        bool isDestroyed() const override {
            return false;
        }

        void destroy() override {}

        void draw(sf::RenderWindow* =nullptr) override;
        
        ~HUD() noexcept final = default;
    };
}
#endif