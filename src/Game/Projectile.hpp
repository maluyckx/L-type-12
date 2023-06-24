#ifndef LTYPE_GAME_ENTITY_PROJECTILE_H
#define LTYPE_GAME_ENTITY_PROJECTILE_H

#include "Abstract/GameEntity.hpp"
#include "Vector2D.hpp"
#include <ctime>
#include <iostream>

namespace Game {
    class PlayerShip;
    /*
     * Entity that will collide with an enemy and damage it, destroyed on impact
     */
    class Projectile final: public GameEntity {
        int _dmg = 2;
        bool _is_hostile = true;
        double _attack_speed = 1;
        bool _destroyed = false;
        PlayerShip* _shooter = nullptr;
    public:
        //Projectile() noexcept=default;
		
		Projectile(std::size_t id, Vector2D pos, EntityType projectile,
                   bool is_hostile, PlayerShip* shooter);
		
        Projectile(std::size_t id, Vector2D pos, ProjectileType projectile,
                   bool is_hostile, PlayerShip* shooter);

        Projectile(const Projectile&) = delete;
        Projectile& operator=(const Projectile&) = delete;

        void setProjectile(ProjectileType type);
        
        static EntityType toEntityType(ProjectileType type);
        
        static ProjectileType toProjectileType(EntityType type);

        inline ProjectileType getProjectileType() { return Projectile::toProjectileType(getType()); }

        static int getDamage(ProjectileType type);

        static Vector2D getDimensions(Game::ProjectileType type);

        static double getAttackSpeed(ProjectileType type);

        static Vector2D getMovementSpeed(ProjectileType Type);

        static std::string getAppearance(ProjectileType type);

        static std::string getPath(ProjectileType type, bool is_hostile=true);

        void draw(sf::RenderWindow* =nullptr) override;

        void onCollision(GameEntity* other,
                         GameInstance* instance = nullptr) override;

        void onUpdate(GameInstance* /*instance = nullptr*/) override;  // Warning fix

        bool isDestroyed() const final {
            return _destroyed;
        }

        void destroy() final {
            _destroyed = true;
        }

        bool isHostile() const {
            return _is_hostile;
        }

        PlayerShip* getShooter() const {
            return _shooter;
        }

        void deleteShooter() {
            _shooter = nullptr;
        }

        ~Projectile() override;
    };

}

#endif // LTYPE_GAME_ENTITY_PROJECTILE_H