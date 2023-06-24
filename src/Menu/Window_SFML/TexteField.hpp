#ifndef LTYPE_TEXTEFIELD_H
#define LTYPE_TEXTEFIELD_H

#include "Texte.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <string.h>

namespace SFML {

        /**
         * A textfield in SFLM with all usefull functions
         * (HORRIBLE)
         */
        class TexteField {       
                sf::RectangleShape _border{};
                Texte* _texte{};
                bool _is_on=false;
                bool _is_select=false;
                unsigned int _colors[3]{};
                size_t _max_size{};
                size_t _where_txt=0;
                bool _is_password{};
        public:
                TexteField() noexcept = default;
                TexteField(const TexteField& b) noexcept = default;
                TexteField& operator=(const TexteField&) noexcept = default;
                
                TexteField(float x, float y, float w, float h, const sf::Uint8* colors, 
                        float size_txt, const sf::Uint8* colors_txt, size_t max_size, bool is_password=false) noexcept;

                void update(char new_char);
                void draw(sf::RenderWindow &window);

                char* getTexte() noexcept;
                void isOn(int x, int y) noexcept;
                void isCLickOn() noexcept;
                bool isSelect() const noexcept {return _is_select;}
                void unselect() noexcept;
                void select() noexcept;

                operator sf::Text() noexcept;

                ~TexteField() noexcept {delete _texte;};
        private:
        };
}



#endif