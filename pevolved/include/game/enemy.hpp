#ifndef __classEnemy__
#define __classEnemy__

#include <SFML/Graphics.hpp>

#include "entity.hpp"

class Character;
class AnimatedSprite;

class Enemy : public Entity {
    public:
        Enemy();
        Enemy( const Enemy & e);

        virtual void Move( const sf::Vector2f & dist);

    private:
        Character * myTarget;
};
#endif
