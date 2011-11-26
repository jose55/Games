#ifndef __CLASS_UNIT__
#define __CLASS_UNIT__

#include <SFML/Graphics.hpp>

#include "Drawer.hpp"
#include "Projectiles.hpp"

#define UNIT_TYPE_LIVING 0
#define UNIT_TYPE_ARTILLERY 1

/*
*   Unit Class
*
*   - version 0.0
*/

class Unit : public Drawer {
    public:
    virtual sf::Vector2f GetPosition() = 0;
    virtual void SetPosition(sf::Vector2f pos) = 0;
    virtual void Fire(float power = 1.0) = 0;
};

#endif
