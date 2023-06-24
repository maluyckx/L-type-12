#include "GameSettings.hpp"
#include <cstring> 
#include <iostream> 

Game::GameSettings::GameSettings(Game::GameSettings::GameMode mode,
                                 Game::GameSettings::Difficulty difficulty,
                                 Game::GameSettings::DropRate drop_rate,
                                 Game::GameSettings::FriendlyFire friendly_fire,
                                 Game::GameSettings::HealthPoints health_points,
                                 const char* level_name) noexcept :
        _mode(mode),
        _difficulty(difficulty),
        _drop_rate(drop_rate),
        _friendly_fire(friendly_fire),
        _health_points(health_points)
        {
            strcpy(_level_name, level_name);
        }
