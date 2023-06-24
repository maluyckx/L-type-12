#include "CollisionResolver.hpp"

/**
 * AABB implementation
 * Masks:
 *  - if 2 entities have the same mask, collision are ignored
 *  - else, a collision is registered and added to a list of collision pair
 * @param all_entities: all the entities in the game currently
 * @param entity_collided: the list of collision pair
 */
void Game::Server::CollisionResolver::physicsUpdate(
    const std::vector<Game::GameEntity*>& all_entities,
    std::vector<CollisionedEntity>& entity_collided) noexcept {
    if (all_entities.size() >= 2) {
        for (std::size_t i = 0; i < all_entities.size() - 1; i++) {
            int              i_width  = all_entities[i]->getWidth();
            int              i_heigth = all_entities[i]->getHeight();
            int              i_x      = static_cast<int>(all_entities[i]->getX());
            int              i_y      = static_cast<int>(all_entities[i]->getY());
            for (std::size_t j        = i + 1; j < all_entities.size(); j++) {
                if (all_entities[i] != all_entities[j]) {
                    int j_width  = all_entities[j]->getWidth();
                    int j_heigth = all_entities[j]->getHeight();
                    int j_x      = static_cast<int>(all_entities[j]->getX());
                    int j_y      = static_cast<int>(all_entities[j]->getY());
                    if (i_x <= j_x + j_width && i_x + i_width >= j_x &&
                        i_y <= j_y + j_heigth && i_y + i_heigth >= j_y) {
                        if (!all_entities[i]->isInSameMask(
                            *(all_entities[j]))) {
                            CollisionedEntity r;
                            r.first  = all_entities[i];
                            r.second = all_entities[j];
                            entity_collided.push_back(r);
                        }
                    } else if (i_x < 0 || i_x + i_width >= WIDTH_WINDOW ||
                               i_y < 0 || i_y + i_heigth >=
                                          HEIGHT_WINDOW) {
                        // left - right - top - bottom
                        // if HUD, skip
                        if (all_entities[i]->getType() == EntityType::HUD)
                            break;
                        CollisionedEntity r;
                        r.first  = all_entities[i];
                        r.second = nullptr;
                        entity_collided.push_back(r);
                        break;
                    }
                }
            }
        }
    }
}