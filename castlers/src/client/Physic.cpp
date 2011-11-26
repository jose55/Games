#include <cmath>

#include "Physic.hpp"

using namespace sf;

Physic::Physic(int h) {
    _position = Vector2f(0, 0);
    _speed = Vector2f(0, 0);
    _size = Vector2f(32, 32);
    _angle = 0;
    _isBreakable = 0;
    _isFree = true;
    _hp = 3;
    _hType = h;
    switch(_hType) {
        default:
        case hbBB: {
            _hitbox = Shape::Rectangle(0, 0, 32, 32, Color::Red);
            _hitbox.SetCenter(16, 16);
            _hb = new HBBB( Vector2f(32, 32));
        } break;
        case hbSphere: {
            _hitbox = Shape::Circle(0, 0, 16, Color::Red);
            _hb = new HBSphere(16);

        } break;
        _hitbox.SetBlendMode(Blend::Add);

    }
}

// Getters
sf::Vector2f Physic::GetSize() {
    if ( _angle == 0 ) return _size;
    else return Vector2f( _size.y, _size.x);
}

void Physic::SetSize(float x, float y) { SetSize(Vector2f(x, y)); }
void Physic::SetSize(sf::Vector2f size) {
    _size = size;
    delete _hb;

    switch(_hType) {
        case hbBB: {
            _hitbox = Shape::Rectangle(0, 0, GetSize().x, GetSize().y, Color::Red, 1.0, Color::White);
            _hitbox.SetCenter(GetSize().x/2, GetSize().y/2);
            _hb = new HBBB( size );
        } break;
        case hbSphere: {
            _hitbox = Shape::Circle(0, 0, sqrtf(size.x*size.y)/2, Color::Red, 1.0, Color::White);
            _hb = new HBSphere( sqrt(size.x*size.y)/2 );
        } break;
    }
    _hitbox.SetBlendMode(Blend::Add);
    _hitbox.EnableOutline(true);
    _hitbox.SetOutlineWidth(1);
}


sf::Shape * Physic::DrawHitBox() {
    _hitbox.SetPosition(_position);
    return &_hitbox;
}

// Movement
float Physic::GetY(int point) { return _position.y + _hb->GetY(point); }
float Physic::GetX(int point) { return _position.x + _hb->GetX(point); }

void Physic::SetPosition(Vector2f pos) {
    _position = pos;
    physicMoved();
}

void Physic::SetPosition(float x, float y) { SetPosition(Vector2f(x, y)); }

void Physic::SetX(float x) { _position.x = x; physicMoved(); }
void Physic::SetY(float y) { _position.y = y; physicMoved(); }

void Physic::Snap(int x, int y) {
    float L = x * 32 + _hitbox.GetCenter().x;
    float H = y * 32 - _hitbox.GetCenter().y;
    _position = Vector2f(L, H);
    physicMoved();
}


void Physic::Move(float x, float y) {
    _position += Vector2f(x, y);
    physicMoved();
}

void Physic::AddVelocity(Vector2f v) { _speed = v; }
sf::Vector2f Physic::GetVelocity() { return _speed; }



// Destruction
void Physic::SetBreakable(bool v) { _isBreakable = v; }
bool Physic::IsBreakable() { return _isBreakable; }
bool Physic::Hit(int d) {
    _hp -= d;
    if ( _hp < 1 ) return true;
    return false;
}



// V.03
void Physic::physicMoved() {}

bool Physic::ShareX(Physic * obj) {
    if ( GetX() >= obj->GetX(1) || GetX(1) <= obj->GetX() ) return false;
    return true;
}

bool Physic::ShareY(Physic * obj) {
    if ( GetY() >= obj->GetY(1) || GetY(1) <= obj->GetY() ) return false;
    return true;
}

void Physic::SetFree(bool v) { _isFree = v; }
bool Physic::IsFree() { return _isFree; }
