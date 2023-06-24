#ifndef LTYPE_GAME_DESTRUCTABLE_OBSTACLE_H
#define LTYPE_GAME_DESTRUCTABLE_OBSTACLE_H

#include <utility>

#include "../Abstract/Destructable.hpp"
#include "../Lootable.hpp"
#include "../Abstract/GameEntity.hpp"
#include "../Abstract/Ship.hpp"
#include "../Abstract/EntityType.hpp"

namespace Game{
    /**
     * Destructable Entity that just moves down and collides with everyone.
     */
    class Obstacle final: public Destructable {
        Lootable _loot=Lootable(EntityType::BonusWeaponMissile);
        bool _collided_with_hud = false;

    public:
        Obstacle(std::size_t id, Game::Vector2D position);

        void draw(sf::RenderWindow* =nullptr) override;

        void onUpdate(GameInstance* instance = nullptr) override;

        void onCollision(Game::GameEntity* other,
                         GameInstance* instance = nullptr) override;
        
        ~Obstacle() noexcept final = default;
    };

}

#endif // LTYPE_GAME_DESTRUCTABLE_OBSTACLE_H