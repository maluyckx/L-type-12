#ifndef LTYPE_GAME_GAMESETTINGS_H
#define LTYPE_GAME_GAMESETTINGS_H

#include "../Utils/Constants.hpp"
#include <string>
#include <cstring>

namespace Game {
    class GameSettings {
    public:
        enum class GameMode {
            Single,
            Coop,
        };
        enum Difficulty : int {
            Easy   = 1,
            Normal = 2,
            Hard   = 3,
        };
        enum DropRate : int {  // chance on 10
            None   = 0,
            Average = 2,
            High   = 5,
        };
        enum FriendlyFire : bool{
            No  = false,
            Yes = true,
        };
        enum HealthPoints : int {
            Few  = 3,
            Some = 5,
            Many = 10,
        };
    private:
        GameMode     _mode;
        Difficulty   _difficulty;
        DropRate     _drop_rate;
        FriendlyFire _friendly_fire;
        HealthPoints _health_points;
        char         _level_name[LEVEL_NAME_MAX_SIZE];
    public:
        GameSettings() noexcept = default;
        GameSettings(GameMode, Difficulty, DropRate, FriendlyFire, HealthPoints, const char*) noexcept;
        ~GameSettings() noexcept = default;

        constexpr GameMode getMode() const noexcept {
            return _mode;
        }

        constexpr Difficulty getDifficulty() const noexcept {
            return _difficulty;
        }

        constexpr DropRate getDropRate() const noexcept {
            return _drop_rate;
        }

        constexpr FriendlyFire getFriendlyFire() const noexcept {
            return _friendly_fire;
        }

        constexpr HealthPoints getHealthPoints() const noexcept {
            return _health_points;
        }
        
        constexpr const char* getLevelName() noexcept {
            return _level_name;
        }
        
        void setLevelName(std::string level) {
        	for (size_t i = 0; i < LEVEL_NAME_MAX_SIZE; i++) {
				if (i < level.size())
				    _level_name[i] = level[i];
				else
				    _level_name[i] = '\0';
			}
        }
    
    };

}


#endif
