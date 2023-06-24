#include "TexteField.hpp"
#include <iostream>


static const std::string CORRECT_CHAR = " &*^%$#@+-_"; // Special char authorized
const std::string MOUSE_POSITION = "|";
const float THICKNESS = 5;

SFML::TexteField::TexteField(float x, float y, float w, float h, const sf::Uint8* colors,
                float size_txt, const sf::Uint8* colors_txt, size_t max_size, bool is_password) 
                noexcept: _max_size(max_size), _is_password(is_password) {
    _texte = new Texte("", static_cast<int>(size_txt), colors_txt);
    _texte->setPosition(x + size_txt/3,
                        static_cast<float>(y + (h / 2) - size_txt / 1.35));
    _border.setPosition(x, y);
    _border.setSize(sf::Vector2f(w, h));
    for (size_t i = 0; i < 3; ++i) _colors[i]=colors[i];
    _border.setFillColor(sf::Color::Transparent);
    _border.setOutlineColor(sf::Color(static_cast<unsigned char>(_colors[0]), static_cast<unsigned char>(_colors[1]),
                                      static_cast<unsigned char>(_colors[2])));
    _border.setOutlineThickness(THICKNESS);
}

void SFML::TexteField::update(char new_char) {
    if (_is_select) {
        if (new_char==8 && _where_txt > 0) {
            std::string tmp_txt = _texte->getTexte();
            tmp_txt.pop_back();
            _where_txt--;
            tmp_txt[_where_txt] = MOUSE_POSITION[0];
            _texte->setTexte(tmp_txt);
        }
        else if ((isalnum(new_char) || CORRECT_CHAR.find(static_cast<char>(new_char)) != std::string::npos) && _where_txt<_max_size){
            std::string tmp_txt = _texte->getTexte();
            tmp_txt[_where_txt] = new_char;
            tmp_txt += MOUSE_POSITION;
            _texte->setTexte(tmp_txt);
            _where_txt++;
        }
       if (_is_password)
            _texte->update('*');
       else
            _texte->update();
    }
}

void SFML::TexteField::draw(sf::RenderWindow &window) {
    window.draw(_border);
    _texte->draw(window);
}

void SFML::TexteField::isOn(int x, int y) noexcept {
    if (_border.getPosition().x < static_cast<float>(x) && static_cast<float>(x) < _border.getPosition().x + _border.getSize().x &&
        _border.getPosition().y < static_cast<float>(y) && static_cast<float>(y) < _border.getPosition().y + _border.getSize().y) {
        _is_on=true;
        _border.setOutlineColor(sf::Color(
                static_cast<unsigned char>(100 - _colors[0]),
                static_cast<unsigned char>(100 - _colors[1]),
                static_cast<unsigned char>(100 - _colors[2])));
    }else {
        _is_on=false;
        _border.setOutlineColor(sf::Color(
                static_cast<unsigned char>(_colors[0]),
                static_cast<unsigned char>(_colors[1]),
                static_cast<unsigned char>(_colors[2])));
    }
}

void SFML::TexteField::isCLickOn() noexcept {
    if (_is_on) {
        _is_select=true;
        if (_texte->getTexte()[_texte->getTexte().size()-1]!='|')
            _texte->setTexte(_texte->getTexte()+MOUSE_POSITION);
        _texte->update();
    } else {
        std::string tmp_txt = _texte->getTexte();
        if (tmp_txt[tmp_txt.size()-1]=='|') {
            tmp_txt.pop_back();
            _texte->setTexte(tmp_txt);
            _texte->update();
        }
        _is_select=false;
    }
    if (_is_password)
        _texte->update('*');
    else
        _texte->update();

}

char* SFML::TexteField::getTexte() noexcept {
    if (!_texte->getTexte().empty() && _texte->getTexte()[_texte->getTexte().size()-1]=='|') {
        std::string tempo_str = tempo_str.substr(0, _texte->getTexte().size()-1);
        return strdup(tempo_str.c_str());
    }
    else
        return strdup(_texte->getTexte().c_str());
}

void SFML::TexteField::unselect() noexcept {
    _is_select=false;
    std::string tmp_txt = _texte->getTexte();
    tmp_txt.pop_back();
    _texte->setTexte(tmp_txt);
    if (_is_password)
        _texte->update('*');
}

void SFML::TexteField::select() noexcept {
    _is_select=true;
    std::string tmp_txt = _texte->getTexte()+ MOUSE_POSITION;
    _texte->setTexte(tmp_txt);
    if (_is_password)
        _texte->update('*');
}