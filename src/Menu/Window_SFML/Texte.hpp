#ifndef LTYPE_TEXTE_H
#define LTYPE_TEXTE_H

#include <SFML/Graphics.hpp>
#include <string>

namespace SFML {

        /** 
         * Simple text in SFML using special fonts
         * Use kongtext free font
        */
        class Texte {       
                std::string _txt{}; 
                sf::Text text{};
                static sf::Font font;
                bool _is_password=false;
        public:
                Texte() noexcept = default;
                explicit Texte(std::string txt) noexcept: _txt(txt) {}
                Texte(std::string txt, int size_txt, const sf::Uint8* colors) noexcept;
                Texte(std::string txt, float x, float y, int size_txt, const sf::Uint8* colors) noexcept;

                inline void update() noexcept {text.setString(_txt);}
                void update(char special) noexcept;
                void draw(sf::RenderWindow &window);

                inline void move(float x, float y) noexcept {text.move(x, y);}
                void zoom(float z) noexcept;
                void setPosition(float x, float y) noexcept;
                inline void setTexte(std::string txt) noexcept {_txt=txt;update();}
                inline std::string getTexte() noexcept {return _txt;}
                inline float getWidth() noexcept {return text.getLocalBounds().width;}
                inline float getHeight() noexcept {return text.getLocalBounds().height;}
                static void setFont() {font.loadFromFile("sprites/fonts/kongtext.ttf");}

                explicit operator sf::Text() noexcept;

                ~Texte() noexcept = default;
        private:
        };
}


#endif