#include "ParticleHandler.hpp"
#include "../../../Utils/Constants.hpp"

void SFML::ParticleHandler::display(sf::RenderWindow* window) {
    for (auto particle: _particles)
        particle->draw(*window);
}

void SFML::ParticleHandler::update(float advance) {
    for (auto particle: _particles)
        particle->animate(advance);
    for (size_t i = 0; i < _particles.size(); ++i) {
        if(_particles[i]->isLastFrame()) {
            delete _particles[i];
            _particles.erase(_particles.begin() + static_cast<ptrdiff_t>(i));
        }
    }
}

void SFML::ParticleHandler::addParticle(SFML::Particle particle, float x, float y, float w, float h, bool reversed) {
    SpriteSimple* sprite{};
    x *= GRAPHIC_X_MULTIPLIER;
    y *= GRAPHIC_Y_MULTIPLIER;
    w *= GRAPHIC_X_MULTIPLIER;
    h *= GRAPHIC_X_MULTIPLIER;
    if(reversed) {
    	x += w;
    	y += h;
    }
    switch (particle) {
        case Particle::ProjectileExplosion:
            sprite = new SpriteSimple(x, y, w, h, "sprites/animations/ProjectileExplosion/explosion_", 21);
            break;
        case Particle::ShipExplosion:
            sprite = new SpriteSimple(x, y, w, h, "sprites/animations/ShipExplosion/explosion_", 29);
            break;
        case Particle::MassiveExplosion:
            sprite = new SpriteSimple(x, y, w, h, "sprites/animations/MassiveExplosion/explosion_", 24);
            break;
        case Particle::BonusWeaponRecovered:
        	addParticle(Particle::BonusHealRecovered, x, y, w, h, reversed);
            sprite = new SpriteSimple(x, y, w, h, "sprites/animations/BonusRecovered/elec_", 4);
            break;
        case Particle::Warp:
        case Particle::BonusHealRecovered:
            sprite = new SpriteSimple(x, y, w, h, "sprites/animations/BonusRecovered/energie_", 8);
            break;
        default:
            break;
    }
    if(reversed) sprite->rotate(180);
    _particles.push_back(sprite);
}