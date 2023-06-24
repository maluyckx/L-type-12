#ifndef LTYPE_GAME_BOSS_H
#define LTYPE_GAME_BOSS_H

#include "Destructable.hpp"
#include <iostream>
#include "../../Utils/Constants.hpp"
#include "../Projectile.hpp"
#include "../Lootable.hpp"

namespace Game {
    /**
     * The biggest of all enemies, they vary a lot in moves, attacks and hp 
     * but they all give a life on death.
     */
    class Boss : public Destructable {

    protected:
        Lootable _loot = Lootable(EntityType::BonusHeal, true);  // garanteed life bonus on death

        Boss(Game::EntityType type, std::size_t id, int w, int h,
             Game::Vector2D pos, std::string style, int hp, std::string path);
        
    public:
        virtual void draw(sf::RenderWindow* =nullptr) override;

        void onUpdate(GameInstance* instance=nullptr) override;

        void onCollision(Game::GameEntity* other,
                         GameInstance* instance = nullptr) override;
                         
        virtual ~Boss() noexcept override = default;
    };

}

#endif // LTYPE_GAME_BOSS_H