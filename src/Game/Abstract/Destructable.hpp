#ifndef LTYPE_GAME_ENTITY_SPACE_OBJECT_DESTRUCTABLE_H
#define LTYPE_GAME_ENTITY_SPACE_OBJECT_DESTRUCTABLE_H

#include "GameEntity.hpp"

namespace Game {
    /**
     * A GameEntity that can be destroyed and has hp.
     */
    class Destructable : public GameEntity { // Abstract
        int  _hp;
        bool _destroyed = false;

    protected:
        Destructable(Game::EntityType type, std::size_t id, int w, int h,
                     Game::Vector2D pos, std::string style, CollisionMask mask,
                     int hp, std::string path) :
            GameEntity(type, id, w, h, pos, std::move(style), mask, std::move(path)), _hp(hp) {};

    public:
        // Getters
        inline bool isAlive() const { return getHp()>0; }
        
        bool isDestroyed() const override { return _destroyed; }

        int getHp() const { return _hp; }

        // Setters
        void setHp(int hp) { _hp = hp; }

        virtual void lostHp(int hp) { _hp -= hp; }

        void destroy() override { _destroyed = true; }

        ~Destructable() noexcept override = default;
    };
}

#endif // LTYPE_GAME_ENTITY_SPACE_OBJECT_DESTRUCTABLE_H