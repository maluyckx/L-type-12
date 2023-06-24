#ifndef LTYPE_GAME_BOSS_ASTEROIDBASE_H
#define LTYPE_GAME_BOSS_ASTEROIDBASE_H

#include "../Abstract/Destructable.hpp"
#include "../Lootable.hpp"
#include "../Abstract/Boss.hpp"
#include "../../Utils/Time.hpp"

namespace Game {
    /*
     * Boss that does't' move, looks like a big asteroid and 
     * is only vulnerable to damage when it makes Swarmlings appear. (when it's not teal)
     */
    class AsteroidBase final : public Boss {
        bool _is_spawning       = false;
        int          _spawn_pos = -1;
        Utils::Timer _cooldown  = Utils::Timer();

    public:
        AsteroidBase(long unsigned int id, int diff, Vector2D position);
        // spawns a bunch of swarmling then wait and start again
        void spawn(GameInstance* instance);
        
        void draw(sf::RenderWindow* = nullptr) override;

        void onUpdate(GameInstance* instance=nullptr) override;
        // only vulnerable to damage when it makes swarmlings appear
        void lostHp(int hp) override { if (_is_spawning) Destructable::lostHp(hp);}

        ~AsteroidBase() noexcept = default;
    };

}

#endif // LTYPE_GAME_BOSS_ASTEROIDBASE_H

