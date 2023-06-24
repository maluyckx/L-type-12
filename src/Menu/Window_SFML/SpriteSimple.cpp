#include "SpriteSimple.hpp"
#include "../../Utils/Constants.hpp"
#include <iostream>

SFML::SpriteSimple::SpriteSimple(float x, float y, float w, float h, std::string path) noexcept {
	_max=1;
	_textures.emplace_back(new sf::Texture());
	_textures[0]->loadFromFile(path+".png");
	_textures[0]->setSmooth(false);	
	_sprite.setScale(w / static_cast<float>(_textures[0]->getSize().x), h /
                                                                        static_cast<float>(_textures[0]->getSize().y));
	_sprite.setTexture(*_textures[0]);
	_sprite.setPosition(x, y);
}

SFML::SpriteSimple::SpriteSimple(float x, float y, float w, float h, std::string path, size_t nb) noexcept: _max(nb) {
	for (size_t i = 0; i < nb; ++i) {
		_textures.emplace_back(new sf::Texture());
		_textures[i]->loadFromFile(path+std::to_string(i)+".png");
		_textures[i]->setSmooth(false);
	}
	_sprite.setScale(w / static_cast<float>(_textures[0]->getSize().x), h /
                                                                        static_cast<float>(_textures[0]->getSize().y));
	_sprite.setTexture(*_textures[0]);
	_sprite.setPosition(x, y);
}

SFML::SpriteSimple::SpriteSimple(float x, float y, float w, float h, std::string path, size_t max_width, bool one_texture) noexcept: _max(max_width) {
	_one=one_texture;
	_textures.emplace_back(new sf::Texture());
	_textures[0]->loadFromFile(path+".png");
	float tmp_scale = h / static_cast<float>(_textures[0]->getSize().y);
	_sprite.setTexture(*_textures[0]);
	_sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(tmp_scale * w), static_cast<int>(tmp_scale * h)));
	_sprite.setScale(tmp_scale, tmp_scale);
	_sprite.setPosition(x, y);
}

void SFML::SpriteSimple::move(float x, float y) {
	_sprite.move(x, y);
}

void SFML::SpriteSimple::setPosition(float x, float y) {
	_sprite.setPosition(x, y);
}

void SFML::SpriteSimple::rotate(float degre) noexcept {
	_sprite.rotate(degre);
}

void SFML::SpriteSimple::draw(sf::RenderWindow &window) {
    window.draw(_sprite);
}

void SFML::SpriteSimple::animate(float advance) noexcept {
	_animation+=advance;
	if (static_cast<size_t>(_animation)==_max) 
		_animation=0;
	if (!_one)
		_sprite.setTexture(*_textures[static_cast<size_t>(_animation)]);
	else {
		float tmp_width = static_cast<float>(_textures[0]->getSize().x) / static_cast<float>(_max);
		_sprite.setTextureRect(sf::IntRect(
                static_cast<int>(static_cast<float>(static_cast<int>(_animation)) * tmp_width), 0,
                static_cast<int>(tmp_width), static_cast<int>(_textures[0]->getSize().y)));
	}
}

bool SFML::SpriteSimple::isOutScreen() noexcept {
	if (_sprite.getPosition().y + _sprite.getScale().y * static_cast<float>(_textures[0]->getSize().y) < 0 ||
		_sprite.getPosition().y > HEIGHT_WINDOW_SFML+100)
		return true;
	if (_sprite.getPosition().x > WIDTH_WINDOW_SFML+200)
		return true;
	return false;
}

SFML::SpriteSimple::~SpriteSimple() noexcept {
	for (auto & _texture : _textures) {
		delete _texture;
	}
}
