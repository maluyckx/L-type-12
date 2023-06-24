#ifndef LTYPE_GAME_COLLISIONRESOLVER_H
#define LTYPE_GAME_COLLISIONRESOLVER_H

#include "../Abstract/GameEntity.hpp"
#include <vector>

namespace Game::Server {

    /**
     * A pair of entity that have collided
     */
    typedef struct CollisionedEntity {
        GameEntity* first;
        GameEntity* second;
    } CollisionedEntity;

    /**
     * A Collision resolver with Axis-Aligned Bounding Boxes (AABB)
     */
    class CollisionResolver {
    public:
        static void
        physicsUpdate(const std::vector<Game::GameEntity*>& all_entities,
                      std::vector<CollisionedEntity>& entity_collided) noexcept;
    };
}

#endif // LTYPE_GAME_COLLISIONRESOLVER_H
