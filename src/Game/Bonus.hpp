#ifndef LTYPE_GAME_ENTITY_SPACE_OBJECT_BONUS_H
#define LTYPE_GAME_ENTITY_SPACE_OBJECT_BONUS_H

#include "Abstract/GameEntity.hpp"
#include <iostream>

namespace Game{
    /**
     * GameEntity that can be picked up by the player for a change of weapon 
     * or a life.
     */
    class Bonus final: public GameEntity {
    private:
        bool _destroyed = false;
        ProjectileType proj_type{};
    public:
        Bonus(EntityType type, size_t id, Vector2D pos);

        void draw(sf::RenderWindow* =nullptr) override;     
        void onUpdate(GameInstance* instance = nullptr) override;
        void onCollision(Game::GameEntity* other,
                         GameInstance* instance = nullptr) override;

        ProjectileType getProjectileBonus() const { return proj_type; }

        bool isDestroyed() const override { return _destroyed; }

        void destroy() override { _destroyed = true; }
        ~Bonus() noexcept final = default;
    };

}

#endif // LTYPE_GAME_ENTITY_SPACE_OBJECT_BONUS_H