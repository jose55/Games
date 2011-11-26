#ifndef __classEntity__
#define __classEntity__

#include <SFML/Graphics.hpp>

#include "physic.hpp"

class AnimatedSprite;

class Entity : public Physic, public sf::Drawable {
    public:
        Entity();
        virtual ~Entity();

        const virtual sf::FloatRect getBB();

        virtual void Render(sf::RenderTarget & target) const;
        virtual void setPos( const sf::Vector2f & pos);
        virtual void Move( const sf::Vector2f & dist);
        virtual int Update();

        // Damage system data
        int myOwner;
        int hit( int damage );
        float getLife();
        inline float getDamage() { return 1.f; } ;

    protected:
        float myLife;
        AnimatedSprite * mySprite;
        sf::Shape myBB;
};

#endif
