#ifndef __CLASS_ART__
#define __CLASS_ART__

#include <SFML/Graphics.hpp>

#include "Unit.hpp"
/*
*   Artillery Class
*
*   - version 0.0
*/

using sf::Sprite;
using sf::Vector2f;

class Artillery : public Unit {
    public:
        Artillery();
        Artillery(Artillery * tmp);
        void Draw();
        void SetBody(Sprite * s);
        void SetCanon(Sprite * s);
        void SetCanonCenter(Vector2f pos);
        void SetCanonPosition(Vector2f pos);
        void CanonRotate(float ang);

        // v0.2
        sf::Vector2f GetPosition();
        void SetPosition( sf::Vector2f pos);
        void LoadProjectile(Projectile * p);
        void Fire(float power);

   private:
        Vector2f _canonPosition;
        float _canonAngle;
        Projectile * _cProjectile;
};

#endif

