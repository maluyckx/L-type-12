#ifndef LTYPE_MENU_ROOMMENU_H
#define LTYPE_MENU_ROOMMENU_H

#include "../../Game/GameSettings.hpp"
#include "../MenuInfo.hpp"

namespace Menu {
    class RoomMenu: protected MenuInfo {
    protected:
        Game::GameSettings::GameMode     _is_solo = Game::GameSettings::GameMode::Single;
        Game::GameSettings::Difficulty   _difficulty = Game::GameSettings::Difficulty::Normal;
        Game::GameSettings::DropRate     _dropping_rate = Game::GameSettings::DropRate::Average;
        Game::GameSettings::FriendlyFire _friendly_fire = Game::GameSettings::FriendlyFire::No;
        Game::GameSettings::HealthPoints _lives = Game::GameSettings::HealthPoints::Some;
        std::string                      _level_name = DEFAULT_LEVEL; // TODO: Remplacer avec le nom du niveau 1 de la campagne
        bool                             _level_name_correct = true;    // PArdéfatu c'est ok (vu que c'est la campagne)
        static char                      _name_level[LEVEL_NAME_MAX_SIZE];
        void start();
        void logoutPlayer2(char* username);
    public:
        static char* getLevelPlayed() { return _name_level; }
        static void  setLevelPlayed(std::string level);
        RoomMenu() noexcept = default;
        ~RoomMenu() noexcept override = default;
    };
}


#endif