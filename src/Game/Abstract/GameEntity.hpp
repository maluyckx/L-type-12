#ifndef LTYPE_GAME_ENTITY_H
#define LTYPE_GAME_ENTITY_H

#include "../Vector2D.hpp"
#include <string>
#include "../Vector2DServer.hpp"
#include "../../Utils/Constants.hpp"  // for terminal colors
#include "EntityType.hpp"
#include <SFML/Graphics.hpp>
#include "../../Menu/Window_SFML/SpriteSimple.hpp"

namespace Game {
    /**
     * The "Team" of the entity, 
     * same teams don't collide with each others.
     */
    enum class CollisionMask {
        HUD           = 0,
        RESISTANCE    = 1,
        BOUNTY_HUNTER = 2,
        EMPIRE        = 3,
        OBSTACLE      = 4,
        PEW_PEW       = 5
    };

    class GameState;

    class GameInstance;

    class PlayerShip;

    /**
     * Everything that exist in the game, on the screen, if a kind of entity.
     */
    class GameEntity { // Abstract
        EntityType     _type{};  // the precise kind of entity it is
        std::size_t    _id{};
        int            _width{}, _height{};
        bool           _is_collisioned = false;
        CollisionMask  _mask           = CollisionMask::EMPIRE;
        
        Vector2D       _pos{};  // coordonate relative to bottom left
        Vector2DServer _pos_serv{};
        // on the server it is the old position, so we don't send update for entities that haven't moved
        
        std::string    _style{};  // text for terminal appearance
        SFML::SpriteSimple* _sprite = nullptr;  // image for graphical appearance
        
    protected:
        Vector2D _speed = Vector2D::down();
        
        GameEntity() noexcept = default;
        // Used to construct ships
        GameEntity(EntityType type, std::size_t id,
                   int width, int height,
                   Vector2D pos, std::string style, CollisionMask mask, std::string path);
        // Used to construct projectiles.
        GameEntity(ProjectileType type, std::size_t id, Vector2D pos, std::string style,
                   bool is_hostile, Vector2D speed, Vector2D dimensions, std::string path);
        // Used to construct bonus.
        GameEntity(EntityType type, std::size_t id, Vector2D pos);

    public:
        GameEntity(const GameEntity&) noexcept = delete;
        GameEntity& operator=(const GameEntity&) noexcept = delete;
        // has a server and client definition, both called at each loop of the game
        virtual void onUpdate(GameInstance* instance = nullptr);
        // will call the right pencil with the right color
        virtual void draw(sf::RenderWindow* =nullptr) = 0;
        // used to draw the entity in terminal window
        static void pencil(GameEntity& entity, TERMINAL_COLORS color);
        // used to draw the entity in graphical window
        static void pencil(GameEntity& entity, sf::RenderWindow* window);

        // Getters
        inline constexpr Vector2D getPosition() noexcept { return _pos; }

        inline constexpr Vector2DServer getPositionServer() noexcept {
            return _pos_serv;
        }

        inline constexpr int getWidth() noexcept { return _width; }

        inline constexpr int getHeight() noexcept { return _height; }

        inline constexpr float getX() noexcept { return _pos.getX(); }

        inline constexpr float getY() noexcept { return _pos.getY(); }

        inline constexpr EntityType getType() { return _type; }
        // used to know which GenericType corresponds to the specific EntityType given 
        // (those GenericType entities generally behave in the same way)
        static GenericType getGenericType(EntityType type);

        inline constexpr std::size_t getId() noexcept { return _id; }

        inline std::string getStyle() noexcept { return _style; }

        virtual bool isDestroyed() const = 0;

        inline constexpr CollisionMask getMask() { return _mask; }

        inline constexpr Vector2D getSpeed() noexcept { return _speed; }

        inline SFML::SpriteSimple* getSprite() noexcept { return _sprite; }

        // Setters
        inline void setPosition(Vector2D pos) {
            _pos_serv = _pos;
            _pos      = pos;
        }

        inline constexpr void
        setPositionServer(Vector2DServer pos_serv) { _pos_serv = pos_serv; }

        inline constexpr void setX(float x) { _pos.setX(x); }

        inline constexpr void setY(float y) { _pos.setY(y); }

        inline constexpr void setWidth(int w) { _width = w; }

        inline constexpr void setHeight(int h) { _height = h; }

        inline constexpr void setId(std::size_t id) noexcept { _id = id; }

        inline constexpr void setSpeed(Vector2D speed) { _speed = speed; }
        // method called when this entity is collisioned with "other" entity
        virtual void onCollision(Game::GameEntity* other, GameInstance*
        instance = nullptr) = 0;

        inline constexpr bool
        isCollisioned() noexcept { return _is_collisioned; }
        // sets the terminal appearance of the object
        void setStyle(std::string style) noexcept { _style = style; }
        // sets the graphical appearance of the object
        void setSprite(std::string path, bool enemy = true, float scale = 1);

        inline constexpr void setMask(CollisionMask mask) { _mask = mask; }

        bool isInSameMask(Game::GameEntity&) noexcept;

        inline void
        setType(Game::EntityType entitytype) noexcept { _type = entitytype; }
        // called when this entity should be destroyed soon
        virtual void destroy() = 0;

        virtual ~GameEntity() noexcept { if(_sprite) delete _sprite; }
        //Used to construct child classes of the corresponding type.
        static GameEntity* from(EntityType type,
                                unsigned long id,
                                Vector2D position,
                                GameState* game_state = nullptr,
                                bool is_hostile = true,
                                PlayerShip* shooter = nullptr);
    };
}

#endif //LTYPE_GAME_ENTITY_H
