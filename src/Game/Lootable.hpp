#ifndef LTYPE_GAME_LOOTABLE_H
#define LTYPE_GAME_LOOTABLE_H

#include "Bonus.hpp"
#include "Abstract/GameEntity.hpp"
#include "Vector2D.hpp"

namespace Game{
    /**
     * It will have a given chance of spawning the corresonding _type of EntityType
     * when spawnBonus will be called, generally at the death of the owner.
     * If the is_garanteed bool is true the loot will always spawn.
     */
    class Lootable {
    private:
        EntityType _type = EntityType::BonusHeal;
        bool is_garanteed = false;
    public:
        Lootable() noexcept = default;

        constexpr Lootable(EntityType t) noexcept: _type(t) {};
        
        constexpr Lootable(EntityType t, bool garanteed) noexcept: _type(t), is_garanteed(garanteed) {};

        inline Lootable(const Lootable &) noexcept = default;  

        inline Lootable(Lootable &&l) noexcept: _type(l.getType()) {
            l.getType();
        }

        inline Lootable &operator=(const Lootable &) noexcept = default;

        inline Lootable &operator=(Lootable &&l) noexcept {
            if (this != &l) {
                _type = l.getType();
                l.getType();
            }
            return *this;
        }

        inline EntityType getType() { return _type; }

        inline void setType(EntityType t) { _type = t; }
        
        void spawnBonus(GameInstance* instance, Vector2D position);

        ~Lootable() noexcept = default;
    };

}

#endif // LTYPE_GAME_LOOTABLE_H