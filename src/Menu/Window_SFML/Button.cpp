#include "Button.hpp"
#include <iostream>
#include <utility>

SFML::Button::Button(float x, float y, float w, float h, std::string path,
				std::string txt) noexcept {
	_texte = new Texte(std::move(txt)); /// for info
	for (int i = 0; i < 2; ++i) {
		_texture[i].loadFromFile(path+std::to_string(i)+".png");
		_texture[i].setSmooth(false);	
	}
	_border.setScale(w / static_cast<float>(_texture[0].getSize().x), h / static_cast<float>(_texture[0].getSize().y));
	_border.setTexture(_texture[0]);
	_border.setPosition(x, y);
}

SFML::Button::Button(float x, float y, float w, float h, std::string path,
				std::string txt, float size_txt, const sf::Uint8* colors_txt) noexcept {
	_texte = new Texte(std::move(txt), static_cast<int>(size_txt), colors_txt);
	_texte->setPosition(x + (w/2-_texte->getWidth()/2),
                        static_cast<float>(y + ((h / 2) - _texte->getHeight() / 1.2)));
	for (int i = 0; i < 2; ++i) {
		_texture[i].loadFromFile(path+std::to_string(i)+".png");
		_texture[i].setSmooth(false);	
	}
	_border.setScale(w / static_cast<float>(_texture[0].getSize().x), h / static_cast<float>(_texture[0].getSize().y));
	_border.setTexture(_texture[0]);
	_border.setPosition(x, y);
}

SFML::Button::Button(float x, float y, float w, float h, std::string path, 
		std::string txt, float size_txt, const sf::Uint8* colors_txt, bool special) noexcept: _is_special(special) {
	_texte = new Texte(std::move(txt), static_cast<int>(size_txt), colors_txt);
	_texte->setPosition(x + (w/2-_texte->getWidth()/2),
                        static_cast<float>(y + ((h / 2) - _texte->getHeight() / 1.2)));
	for (int i = 0; i < 2; ++i) {
		_texture[i].loadFromFile(path+std::to_string(i)+".png");
		_texture[i].setSmooth(false);	
	}
	_border.setTextureRect(sf::IntRect(0, 0, static_cast<int>(_texture[0].getSize().x)/2, static_cast<int>(_texture[0].getSize().y)));
	_border.setScale(2 * w / static_cast<float>(_texture[0].getSize().x), h / static_cast<float>(_texture[0].getSize().y));

	_border.setTexture(_texture[0]);
	_border.setPosition(x, y);
}

void SFML::Button::update(float x, float y) {
	_border.move(x, y);
	_texte->move(x, y);
}

void SFML::Button::draw(sf::RenderWindow &window) {
    window.draw(_border);
	_texte->draw(window);
}

void SFML::Button::isOn(int x, int y) noexcept {
	float tmp_width = _is_special ? static_cast<float>(_texture[0].getSize().x) / 2.f : static_cast<float>(_texture[0].getSize().x);
	if (_border.getPosition().x < static_cast<float>(x) && static_cast<float>(x) < _border.getPosition().x + tmp_width * _border.getScale().x &&
        _border.getPosition().y < static_cast<float>(y) && static_cast<float>(y) < _border.getPosition().y + static_cast<float>(_texture[0].getSize().y) * _border.getScale().y) {
		_is_on=true;
		_border.setTexture(_texture[1]);
		if (_is_special) {
			if (_is_select)
				_border.setTextureRect(sf::IntRect(static_cast<int>(tmp_width), 0, static_cast<int>(tmp_width),
                                                   static_cast<int>(_texture[0].getSize().y)));
			else
				_border.setTextureRect(sf::IntRect(0, 0, static_cast<int>(tmp_width),
                                                   static_cast<int>(_texture[0].getSize().y)));
		}
	}else {
		_is_on=false;
		_border.setTexture(_texture[0]);
		if (_is_special) {
			if (_is_select)
				_border.setTextureRect(sf::IntRect(static_cast<int>(tmp_width), 0, static_cast<int>(tmp_width),
                                                   static_cast<int>(_texture[0].getSize().y)));
			else
				_border.setTextureRect(sf::IntRect(0, 0, static_cast<int>(tmp_width),
                                                   static_cast<int>(_texture[0].getSize().y)));
		}
	}
}

bool SFML::Button::isClickOn() noexcept {
	if (_is_on) {
		_is_select= !_is_select;
		if (_is_special){
			float tmp_width = _is_special ? static_cast<float>(_texture[0].getSize().x) / 2 : static_cast<float>(_texture[0].getSize().x);
			if (_is_select)
				_border.setTextureRect(sf::IntRect(static_cast<int>(tmp_width), 0, static_cast<int>(tmp_width),
                                                   static_cast<int>(_texture[0].getSize().y)));
			else
				_border.setTextureRect(sf::IntRect(0, 0, static_cast<int>(tmp_width),
                                                   static_cast<int>(_texture[0].getSize().y)));
		}
		return true;
	}
	return false;
}

void SFML::Button::unselect() noexcept {
	_is_select=false;
	float tmp_width = static_cast<float>(_texture[0].getSize().x) / 2;
	_border.setTextureRect(sf::IntRect(0, 0, static_cast<int>(tmp_width), static_cast<int>(_texture[0].getSize().y)));
}

void SFML::Button::select() noexcept {
	_is_select=true;
			float tmp_width = static_cast<float>(_texture[0].getSize().x) / 2;
	_border.setTextureRect(sf::IntRect(static_cast<int>(tmp_width), 0, static_cast<int>(tmp_width),
                                       static_cast<int>(_texture[0].getSize().y)));
}