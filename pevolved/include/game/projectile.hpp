#ifndef __classProjectile__
#define __classProjectile__

#include <game/entity.hpp>

class AnimatedSprite;

class Projectile : public Entity {
    public:
        Projectile();
        Projectile(const Projectile & p, int angle);
        ~Projectile();

        int Update();

    private:
        int myAngle;
        sf::Clock myExistence;
};

#endif
