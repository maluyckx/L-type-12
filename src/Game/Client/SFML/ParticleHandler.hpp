#ifndef LTYPE_GAME_CLIENT_PARTICLEHANDLER_H
#define LTYPE_GAME_CLIENT_PARTICLEHANDLER_H

#include "../../../Menu/Window_SFML/SpriteSimple.hpp"
#include <SFML/Graphics.hpp>

namespace SFML {
    /**
     * Different kinds of particles this Handler can make appear.
     */
    enum class Particle {
        ProjectileExplosion,
        ShipExplosion,
        MassiveExplosion,
        BonusWeaponRecovered,
        BonusHealRecovered,
        DragonFire,
        Warp
    };
    /**
     * SFML GRAPHICAL WINDOW ONLY !
     * Makes all the particles appear and disapear when the animations are over.
     */
    class ParticleHandler {
        std::vector<SpriteSimple*> _particles{};

    public:
    	void update(float advance);
    	
        void addParticle(Particle particle, float x, float y, float w=1, float h=1, bool reversed=true);

        void display(sf::RenderWindow* window);
    };

}

#endif // LTYPE_GAME_CLIENT_PARTICLEHANDLER_H