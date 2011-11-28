#include <game/physic.hpp>


Physic::Physic() {
    myGeo = geo_ground;
    myWall = wall_none;
}

const sf::Vector2f Physic::getVel() { return myVel; }
const sf::Vector2f Physic::getSize() { return mySize; }
const sf::Vector2f Physic::getPos() { return myPos; }

const sf::FloatRect Physic::getBB() { return sf::FloatRect(myPos.x, myPos.y, myPos.x+mySize.x, myPos.y+mySize.y); }

void Physic::setVel( const sf::Vector2f & vel) { myVel = vel; }
void Physic::setSize( const sf::Vector2f & size) { mySize = size; }
void Physic::setPos( const sf::Vector2f & pos) { myPos = pos; }
