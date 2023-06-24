#include "Input.hpp"

void Game::Input::updateInputTable(const char* inputs) {
    for (int i = 0; inputs[i] != '\0' && i < INPUT_MAX_SIZE; ++i) {
        auto key = static_cast<GAME_KEYS>(inputs[i]);
        switch (key) {
            case GAME_KEYS::UP_P1: {
                _table |= KeyFlags::Player1_Up;
                break;
            }
            case GAME_KEYS::DOWN_P1: {
                _table |= KeyFlags::Player1_Down;
                break;
            }
            case GAME_KEYS::LEFT_P1: {
                _table |= KeyFlags::Player1_Left;
                break;
            }
            case GAME_KEYS::RIGHT_P1: {
                _table |= KeyFlags::Player1_Right;
                break;
            }
            case GAME_KEYS::SHOOT_P1: {
                _table |= KeyFlags::Player1_Shoot;
                break;
            }
            case GAME_KEYS::UP_P2: {
                _table |= KeyFlags::Player2_Up;
                break;
            }
            case GAME_KEYS::DOWN_P2: {
                _table |= KeyFlags::Player2_Down;
                break;
            }
            case GAME_KEYS::LEFT_P2: {
                _table |= KeyFlags::Player2_Left;
                break;
            }
            case GAME_KEYS::RIGHT_P2: {
                _table |= KeyFlags::Player2_Right;
                break;
            }
            case GAME_KEYS::SHOOT_P2: {
                _table |= KeyFlags::Player2_Shoot;
                break;
            }
            case GAME_KEYS::PAUSE: {
                _table |= KeyFlags::Pause;
                break;
            }
            case GAME_KEYS::RESUME: {
                _table |= KeyFlags::Resume;
                break;
            }
            case GAME_KEYS::QUIT: {
                _table |= KeyFlags::Quit;
                break;
            }
        }
    }
}

void Game::Input::clearInputTable() {
    _table = 0u;
}

bool Game::Input::isKeyDown(GAME_KEYS key) const {
    switch (key) {
        case GAME_KEYS::UP_P1:
            return _table & KeyFlags::Player1_Up;
        case GAME_KEYS::DOWN_P1:
            return _table & KeyFlags::Player1_Down;
        case GAME_KEYS::LEFT_P1:
            return _table & KeyFlags::Player1_Left;
        case GAME_KEYS::RIGHT_P1:
            return _table & KeyFlags::Player1_Right;
        case GAME_KEYS::SHOOT_P1:
            return _table & KeyFlags::Player1_Shoot;
        case GAME_KEYS::UP_P2:
            return _table & KeyFlags::Player2_Up;
        case GAME_KEYS::DOWN_P2:
            return _table & KeyFlags::Player2_Down;
        case GAME_KEYS::LEFT_P2:
            return _table & KeyFlags::Player2_Left;
        case GAME_KEYS::RIGHT_P2:
            return _table & KeyFlags::Player2_Right;
        case GAME_KEYS::SHOOT_P2:
            return _table & KeyFlags::Player2_Shoot;
        case GAME_KEYS::PAUSE:
            return _table & KeyFlags::Pause;
        case GAME_KEYS::RESUME:
            return _table & KeyFlags::Resume;
        case GAME_KEYS::QUIT:
            return _table & KeyFlags::Quit;
        default:
            return _table & KeyFlags::Quit; // Waring idk
    }
}
