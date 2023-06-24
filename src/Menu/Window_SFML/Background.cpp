#include "Background.hpp"

SFML::Background::Background(std::string path) noexcept {
        back_texture.loadFromFile(path+".png");
        back_texture.setSmooth(false);

        back_sprite1.setTexture(back_texture);
        back_sprite2.setTexture(back_texture);

        back_sprite1.setPosition(0, 870-static_cast<float>(back_texture.getSize().y));
        back_sprite2.setPosition(0, 870-static_cast<float>(back_texture.getSize().y*2));
        bg1_y = back_sprite1.getPosition().y;
        bg2_y = back_sprite2.getPosition().y;
}

void SFML::Background::update(float speed) {
	bg1_y += speed;
	bg2_y += speed;

	if (bg1_y == 870) 
		bg1_y=870-static_cast<float>(back_texture.getSize().y*2);
	else if (bg2_y == 870)
		bg2_y=870-static_cast<float>(back_texture.getSize().y*2);

    back_sprite1.setPosition(0, bg1_y); 
    back_sprite2.setPosition(0, bg2_y); 
}

void SFML::Background::draw(sf::RenderWindow &window) {
        window.draw(back_sprite1);
        window.draw(back_sprite2);
}