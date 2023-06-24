#ifndef LTYPE_BUTTON_H
#define LTYPE_BUTTON_H

#include "Texte.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

namespace SFML {


        /**
         * A button in SFML with all usefull functions 
         * super cool :)
         */
        class Button {        
        sf::Texture _texture[2];
        sf::Sprite _border{};
        Texte* _texte{};
        bool _is_on=false;
        bool _is_select=false;
        bool _is_special=false;

        public:
                Button() noexcept = default;
                Button(const Button& b) noexcept = default;
                Button& operator=(const Button&) noexcept = default;
                Button(float x, float y, float w, float h, std::string path, // Just a image, txt is for information
                        std::string txt) noexcept;
                Button(float x, float y, float w, float h, std::string path, 
                        std::string txt, float size_txt, const sf::Uint8* colors_txt) noexcept;
                Button(float x, float y, float w, float h, std::string path, 
                        std::string txt, float size_txt, const sf::Uint8* colors_txt, bool special) noexcept;
                
                void update(float x, float y);
                void draw(sf::RenderWindow &window);

                std::string getTxt() noexcept {return _texte->getTexte();}
                inline void setTxt(std::string txt) noexcept {_texte->setTexte(std::move(txt));}
                
                void isOn(int x, int y) noexcept;
                bool isClickOn() noexcept;
                bool isSelect() const noexcept {return _is_select;}
                void unselect() noexcept;
                void select() noexcept;

                ~Button() noexcept {delete _texte;};
        private:
        };
}


#endif