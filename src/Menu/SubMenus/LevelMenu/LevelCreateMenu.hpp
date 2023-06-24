#ifndef LTYPE_MENU_LEVELCREATEMENUSFML_H
#define LTYPE_MENU_LEVELCREATEMENUSFML_H

#include "../../BaseMenuSFML.hpp"
#include "../ConnectionMenu/ConnectionInfo.hpp"
#include "../../Window_SFML/SpriteSimple.hpp"

namespace Menu {

    struct Mob_pos{
        sf::Sprite* sprite{};
        float width{};
        float height{};
        std::string name{};
    };

    enum ChoicedMob {
        SWARMLING,
        BATTLESHIP,
        CRUISER,
        ESCORTER,
        LASERGUN,
        OBSTACLE,
        BOSS_POULPE,
        BOSS_DRAGON,
        BOSS_ENTERPRISE,
        BOSS_ASTEROID,
        BACKGROUND,
        // KIRBY,
        // NUMBER doit tjrs être en dernier
        _NUMBER  // Le nombre de mobs dans la liste, est utilisé pour initialisé le vecteur de textures
    };

    class LevelCreateMenu : public BaseMenuSFML, public ConnectionInfo {
        sf::View    _view_scroll{};
        int         _factor_time = 2; // Length of the level
        char*       _level_name{};
        ChoicedMob  _current_choice_mob = ChoicedMob::SWARMLING;
        float       _current_scroll_value = 0;
        sf::Texture* _texture_mob[ChoicedMob::_NUMBER] = {};
        std::vector<Mob_pos> _mobs = {};
        bool _boss_posed = false;
        bool _mode_erase = false;
        sf::Sprite _mouse{};
        sf::Sprite _background_view{};
        float viewport = 1;
        bool _is_kirby_cutting = false;
    public:
        LevelCreateMenu() noexcept;

        // BASICS
        void initAll() noexcept override;
        void display() override;
        MNAME windowInputs(sf::Event &event) noexcept override;
        void animation() noexcept override;


        void setTextError() noexcept override;
        bool testValues() noexcept override;

        // Spécifiques à la classe
        Mob_pos createSpriteFromChoice() noexcept;
        void writeIntoFile() noexcept;
        bool collisionResolverEditor();
        void eraser() noexcept;
        void deleteMob(std::size_t) noexcept;
        bool isCurrentlyABoss();
        void setTextBeginEnd() noexcept;
        Game::EntityType getTypeFromName(std::string name);
        sf::Vector2f getPositionsInView();

        // Move & copy
        LevelCreateMenu(const LevelCreateMenu& lcm) : BaseMenuSFML(lcm) {
            for (size_t i = 0; i < ChoicedMob::_NUMBER; i++) _texture_mob[i] = lcm._texture_mob[i];    
        }
        
        LevelCreateMenu& operator=(const LevelCreateMenu& lcm) = delete;

        ~LevelCreateMenu() noexcept override {
            for (auto & i : _texture_mob) i = nullptr;
            _window->setMouseCursorVisible(true);
        };
    };
}
sf::Vector2f getDimensionFromEntity(Menu::ChoicedMob mob);

#endif // LTYPE_MENU_LEVELCREATEMENUSFML_H
