#include "HUD.hpp"
#include <ncurses.h>
#include "Abstract/EntityType.hpp"
#include "Server/GameInstance.hpp"
#include "../Utils/ColorsSFML.hpp"

Game::HUD::HUD(unsigned long id,
               const char* username_player1,
               const char* username_player2) :

    GameEntity(EntityType::HUD,
               id,
               EntityDimensions::HUD_WIDTH,
               EntityDimensions::HUD_HEIGHT,
               Vector2D(0, HEIGHT_WINDOW - EntityDimensions::HUD_HEIGHT),
               "",
               CollisionMask::HUD,
               "sprites/hud/player1_bar"),
    _player1_hp(0),     
    _player2_hp(0),
    _boss_hp(BOSS_HP_UNDEFINED),
    _boss_hp_max(40),
    _player_hp_max(3),
    _player1_score(0),
    _player2_score(0),
    _isPaused(false) {
    strcpy(_username_player1, username_player1);
    strcpy(_username_player2, username_player2);
}

Game::HUD::HUD(unsigned long id,
               const char* username_player1,
               const char* username_player2,
               int life_max_player) :

    HUD(id, username_player1, username_player2) {
    _player_hp_max = life_max_player;
}

void Game::HUD::displayPause(sf::RenderWindow* window) {
    if (_isPaused) {
        if(window) {
            _text.setTexte("PRESS O TO QUIT !");
            _text.setPosition(33*GRAPHIC_X_MULTIPLIER, 40*GRAPHIC_Y_MULTIPLIER);
            _text.draw(*window);
            _text.setTexte("PRESS M TO RESUME !");
            _text.setPosition(68*GRAPHIC_X_MULTIPLIER, 40*GRAPHIC_Y_MULTIPLIER);
            _text.draw(*window);
        } else {
            mvprintw(40, 33, "PRESS O TO QUIT !");
            mvprintw(40, 68, "PRESS M TO RESUME !");
        }
    } else {
        if(window) {
            _text.setTexte("PRESS P TO PAUSE !");
            _text.setPosition(50*GRAPHIC_X_MULTIPLIER, 40*GRAPHIC_Y_MULTIPLIER);
            _text.draw(*window);
        } else
            mvprintw(40, 50, "PRESS P TO PAUSE !");
    }
}

void Game::HUD::drawLife(int player, int life, int life_max, sf::RenderWindow* window) noexcept {
    const int life_bar_size = 25;   // Taille de la barre affichée
    float     ratio         =
                  static_cast<float>(life) / static_cast<float>(life_max);
    int       line_y        = 38;
    // Choix de la couleur d'affichage (en fonction du ratio)
    int color;
    if(!window) {
        if (ratio == 0) color = 1; // Mort (noir & blanc)
        else if (ratio * 10 >= 7) color = 4;    // Vert
        else if (ratio * 10 < 4) color = 2;     // Rouge
        else color = 3; // Orange
        attron(COLOR_PAIR(color));
    } else {
        if (ratio * 10 >= 7) _player_hp.setColor(COLORS_SFML::GREEN);
        else if (ratio * 10 < 4) _player_hp.setColor(COLORS_SFML::RED);
        else _player_hp.setColor(COLORS_SFML::ORANGE);
    }
    if (life != 0) {
        // Dessine les barres de vie en fonction de la vie restante
        if (player == 1) {
            for (int i = 0; static_cast<float>(i) < ratio * life_bar_size; i++) {
                if(window) {
                    _player_hp.setPosition(static_cast<float>(8+i)*GRAPHIC_X_MULTIPLIER, static_cast<float>(line_y)*GRAPHIC_Y_MULTIPLIER);
                    _player_hp.draw(*window);
                } else 
                    mvaddch(line_y, 8 +i, ACS_CKBOARD);
            }
        } else if (player == 2) {
        	if(window) _player2_bar.draw(*window);
            for (int i = 0; static_cast<float>(i) < life_bar_size * ratio; i++) {
                if(window) {
                    _player_hp.setPosition(static_cast<float>(112 + (3 - static_cast<int>(life_bar_size * ratio)) +i)*GRAPHIC_X_MULTIPLIER, static_cast<float>(line_y)*GRAPHIC_Y_MULTIPLIER);
                    _player_hp.draw(*window);
                } else 
                    mvaddch(line_y, 112 + (3 - static_cast<int>(life_bar_size * ratio)) +i,
                        ACS_CKBOARD);
            }
        }
    } else {
        int decal = 8;
        if (player == 2) decal = 111;
        if(window) {
            _text.setTexte("DEAD");
            _text.setPosition(static_cast<float>(decal)*GRAPHIC_X_MULTIPLIER, static_cast<float>(line_y)*GRAPHIC_Y_MULTIPLIER);
            _text.draw(*window);
        }
        else mvprintw(line_y, decal, "DEAD");
    }
    attroff(COLOR_PAIR(color));
}

void Game::HUD::drawLifeBoss(int life, int life_max, sf::RenderWindow* window) noexcept {
    const int life_bar_size = 30;   // Taille de la barre affichée
    float ratio = static_cast<float>(life) / static_cast<float>(life_max);
    attron(COLOR_PAIR(5));
    if (life != 0) {
        if(window) _boss_bar.draw(*window);
        for (int i = 0; static_cast<float>(i) < life_bar_size * ratio; i++) {
            // Dessine les barres de vie en fonction de la vie restante
            if(window) {
                _hp_boss.setPosition(static_cast<float>(WIDTH_WINDOW / 2 - life_bar_size / 2 + i - 6)*GRAPHIC_X_MULTIPLIER, 36.f*GRAPHIC_Y_MULTIPLIER);
                _hp_boss.draw(*window);
            } else mvaddch(36, static_cast<int>(WIDTH_WINDOW / 2 - life_bar_size / 2 + i - 6),
                    ACS_CKBOARD);
        }
    }
    attroff(COLOR_PAIR(5));
}

void Game::HUD::drawScore(int player, int score, sf::RenderWindow* window) {
    attron(COLOR_PAIR(1));
    if (player == 1){
        if(window) {
            _text.setTexte("Score: " + std::to_string(score));
            _text.setPosition(8*GRAPHIC_X_MULTIPLIER, 40*GRAPHIC_Y_MULTIPLIER);
            _text.draw(*window);
        }
        else mvprintw(40, 8, "Score: %d", score);
    } else{
        if(window) {
            _text.setTexte("Score: " + std::to_string(score));
            _text.setPosition(107*GRAPHIC_X_MULTIPLIER, 40*GRAPHIC_Y_MULTIPLIER);
            _text.draw(*window);
        }
        else mvprintw(40, 107, "Score: %d", score);
    }
    attroff(COLOR_PAIR(1));
}

void Game::HUD::draw(sf::RenderWindow* window) {
    if(window) GameEntity::pencil(*this, window);
    else {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_RED);
        init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(4, COLOR_GREEN, COLOR_GREEN);
        init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
        attron(COLOR_PAIR(1));
    }
    for (int i = HEIGHT_WINDOW - EntityDimensions::HUD_HEIGHT + 1;
        i < HEIGHT_WINDOW; i++) {
        move(i, 0);
        clrtoeol();
    }
    for (std::size_t i = 0; i < EntityDimensions::HUD_WIDTH; i++)
        mvaddch(HEIGHT_WINDOW - EntityDimensions::HUD_HEIGHT, static_cast<int>(i),
                ACS_HLINE); // Dessine la barre de séparation
    if (getBossHp() != BOSS_HP_UNDEFINED)
        drawLifeBoss(getBossHp(), getBossHpMax(), window);
    displayPause(window);
    attroff(COLOR_PAIR(1));
    drawLife(1, _player1_hp, getPlayerHpMax(), window);
    drawScore(1, _player1_score, window);
    if (strcmp(_username_player2, "") != 0) {
        drawLife(2, _player2_hp, getPlayerHpMax(), window);
        drawScore(2, _player2_score, window);
    }
    if(window) {
    	_text.setTexte(_username_player1);
        _text.setPosition(static_cast<int>(8*GRAPHIC_X_MULTIPLIER), static_cast<int>(HEIGHT_WINDOW / 1.13*GRAPHIC_Y_MULTIPLIER));
        _text.draw(*window);
        _text.setTexte(_username_player2);
        _text.setPosition(static_cast<float>(115 - strlen(_username_player2)) * GRAPHIC_X_MULTIPLIER, static_cast<float>(HEIGHT_WINDOW / 1.13) * GRAPHIC_Y_MULTIPLIER);
        _text.draw(*window);
    } else {
    	mvprintw(static_cast<int>(HEIGHT_WINDOW / 1.13), 8, _username_player1);
    	mvprintw(static_cast<int>(HEIGHT_WINDOW / 1.13), 115 - static_cast<int>(strlen(_username_player2)),
            _username_player2);
    }
}


#ifdef SERVER
void Game::HUD::onUpdate(GameInstance* instance) {
    auto game_state  = instance->getGameState();
    auto playership1 = game_state->getPlayerShip(true);
    auto playership2 = game_state->getPlayerShip(false);
    auto boss        = game_state->getBoss();
    _player1_hp    = playership1 ? playership1->getLife() : _player1_hp;
    _player2_hp    = playership2 ? playership2->getLife() : _player2_hp;
    _player1_score = playership1 ? playership1->getScore() : _player1_score;
    _player2_score = playership2 ? playership2->getScore() : _player2_score;
    if(boss && boss->getY() >= -0.5f)
        _boss_hp   = boss->getHp();
    else
    	_boss_hp   = BOSS_HP_UNDEFINED;
}
#else
void Game::HUD::onUpdate(GameInstance*) {}
#endif

void Game::HUD::updateContent(char* username_player1, char* username_player2,
                              int player1_hp, int player2_hp, int boss_hp,
                              int player1_score, int player2_score,
                              int player_max_hp,
                              int boss_max_hp) {
    strcpy(_username_player1, username_player1);
    strcpy(_username_player2, username_player2);
    _player1_hp    = player1_hp;
    _player2_hp    = player2_hp;
    _boss_hp       = boss_hp;
    _player1_score = player1_score;
    _player2_score = player2_score;
    _player_hp_max = player_max_hp;
    _boss_hp_max   = boss_max_hp;
	_player_hp.setSize(static_cast<float>(10/_player_hp_max)*GRAPHIC_X_MULTIPLIER, GRAPHIC_Y_MULTIPLIER);
	_hp_boss.setSize(GRAPHIC_X_MULTIPLIER, GRAPHIC_Y_MULTIPLIER);
}
