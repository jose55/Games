#ifndef __CLASS_PROJECTILES__
#define __CLASS_PROJECTILES__

#include <SFML/Graphics.hpp>

#include "Physic.hpp"

class Projectile : public Physic, public sf::Drawable  {
    public:
        Projectile();
        void Render(sf::RenderTarget& target) const; // inherit:Drawable

    protected:
        void physicMoved();

    private:
        sf::Sprite p;
};

#endif
