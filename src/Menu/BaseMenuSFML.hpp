#ifndef LTYPE_MENU_BASEMENUSFML_H
#define LTYPE_MENU_BASEMENUSFML_H

#include "BaseMenu.hpp"
#include "Window_SFML/Button.hpp"
#include "Window_SFML/Stars.hpp"
#include "Window_SFML/Background.hpp"
#include "Window_SFML/Texte.hpp"
#include "Window_SFML/TexteField.hpp"
#include "Window_SFML/SpriteSimple.hpp"

#include "../Game/Client/GameClient.hpp"
#include "../Utils/Constants.hpp"
#include "../Game/Command.hpp"

#include <SFML/Graphics.hpp>
#include "../Utils/ColorsSFML.hpp"

namespace Menu {


    /**
     * 
     * Abstract menu with baics methode to use the Menu with POLYMORPHISME in SFML
     * herited by all SfmlMenus, use Window_SFML subclass for draw everything on the menu
     * All of Window_SFML class are simplifactions of SFML for simple use of it
     * like a lib
     * We're proud of it.
    */
    class BaseMenuSFML: public BaseMenu {
    protected:
        static sf::RenderWindow* _window;
        // cool view between frames
        // Are static protected cause all menus SFML have the same background/stars/objects
        // is usless to change pointer for each menu and animation doesn't restart for each menu
        static SFML::Background* _background;
        static SFML::Stars* _stars;
        static std::vector<SFML::SpriteSimple*> _bg_sprites;
        static int _random_spawn;

        // All Objects to display for each menu
        std::vector<SFML::Button*> _buttons{};
        std::vector<SFML::TexteField*> _txt_fields{};
        std::vector<SFML::Texte*> _textes{};
        std::vector<SFML::SpriteSimple*> _sprites{};

        BaseMenuSFML() noexcept = default;
        BaseMenuSFML(const BaseMenuSFML&) noexcept = default;
        BaseMenuSFML& operator=(const BaseMenuSFML&) noexcept = default;
    public:
        // initializes everything, window, buttons, Win and text sprite
        void initAll() noexcept override = 0;
        // Display everything each frame
        void display() override;
        // Get inputs and return what he need to do
        MNAME inputs() override;
        virtual MNAME windowInputs(sf::Event &event) noexcept = 0; 

        void animation() noexcept override;

        void quitMenu() noexcept override;

        static void setWindow(sf::RenderWindow* wd) noexcept {_window=wd;}

        ~BaseMenuSFML() noexcept override;
    
        private:
        void generateRandom()noexcept;
    };
}

#endif