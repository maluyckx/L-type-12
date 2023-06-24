#include "Texte.hpp"
#include <iostream>
#include <utility>
sf::Font SFML::Texte::font = sf::Font();

SFML::Texte::Texte(std::string txt, int size_txt, const sf::Uint8* colors) noexcept {
    _txt=std::move(txt);
    text.setFont(font); 
    text.setString(_txt);
    text.setCharacterSize(static_cast<unsigned int>(size_txt));
    text.setFillColor(sf::Color(colors[0], colors[1], colors[2]));
}

SFML::Texte::Texte(std::string txt, float x, float y, int size_txt, const sf::Uint8* colors) noexcept {
    _txt=std::move(txt);
    text.setFont(font); 
    text.setString(_txt);
    text.setCharacterSize(static_cast<unsigned int>(size_txt));
    text.setFillColor(sf::Color(colors[0], colors[1], colors[2]));
    text.setPosition(x, y);
}

void SFML::Texte::zoom(float z) noexcept {
    float actual_w = text.getLocalBounds().width*text.getScale().x;
    float actual_h = text.getLocalBounds().height*text.getScale().y;
    text.scale(z, z);
    text.move((actual_w-text.getLocalBounds().width*text.getScale().x)/2,
              (actual_h-text.getLocalBounds().height*text.getScale().y)/2);
}

void SFML::Texte::setPosition(float x, float y) noexcept {
    text.setPosition(x, y);
}

void SFML::Texte::draw(sf::RenderWindow &window) {
    window.draw(text);
}

void SFML::Texte::update(char special) noexcept {
    std::string tmp_txt;
    for (size_t i = 0; i < _txt.length(); i++) {
        tmp_txt+=special;
    } 
    if (_txt[_txt.length()-1]=='|')
        tmp_txt[tmp_txt.length()-1]='|';
    text.setString(tmp_txt);   
}

SFML::Texte::operator sf::Text() noexcept {
    return text;
}