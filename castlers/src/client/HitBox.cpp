#include "HitBox.hpp"

HBBB::HBBB( sf::Vector2f size) { _mySize = size; }

float HBBB::GetY(int p) {
    if ( p == 0 ) return -_mySize.y/2;
    return _mySize.y/2;
}

float HBBB::GetX(int p) {
    if ( p == 0 ) return -_mySize.x/2;
    return _mySize.x/2;
}


HBSphere::HBSphere(float size) { _mySize.x = size; }

float HBSphere::GetY(int p) {
    switch(p) {
        default: return -_mySize.x; break;
        case mid: return 0; break;
        case p2: return _mySize.x; break;
    }
}

float HBSphere::GetX(int p) { return GetY(p); }
