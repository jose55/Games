/*
*   Artillery Class (inherit Unit)
*
*   - version 0.0
*/
#include <math.h>
#include <cstdio>

#include "Artillery.hpp"
/*
*
*/

Artillery::Artillery() {
    _canonAngle = 0.f;
    _canonPosition = Vector2f(0, 0);
    _cProjectile = 0x00;
}

Artillery::Artillery(Artillery * tmp) {
    AddSprite("body", tmp->GetSprite("body"));
    AddSprite("canon", tmp->GetSprite("canon"));
    SetCanonCenter(tmp->GetSprite("canon")->GetCenter());
    SetCanonPosition(_canonPosition = tmp->_canonPosition);
    _canonAngle = 0.f;
    _cProjectile = 0x00;
}

void Artillery::Draw() {
    Sprite * sprCanon = GetSprite("canon");
    Sprite * sprBody = GetSprite("body");
    if ( sprCanon )
        _win->Draw(*sprCanon);
    if ( sprBody )
        _win->Draw(*sprBody);
}

void Artillery::SetBody(Sprite * s) {
    if ( s == 0x00 ) return;
    s->SetCenter(0, s->GetSize().y);
    AddSprite("body", s);
}

void Artillery::SetCanon(Sprite * s) { AddSprite("canon", s); }
void Artillery::SetCanonCenter(Vector2f pos) { GetSprite("canon")->SetCenter(pos); }

/* Version 02 : Clean */

//Unit::GetPosition
Vector2f Artillery::GetPosition() {
    return GetSprite("body")->GetPosition();
}

//Unit::SetPosition
void Artillery::SetPosition( Vector2f pos) {
    GetSprite("body")->SetPosition( pos);
    GetSprite("canon")->SetPosition(pos + _canonPosition);
}

//Artillery::SetCanonPosition
void Artillery::SetCanonPosition(Vector2f pos) { GetSprite("canon")->SetPosition( GetPosition() + (_canonPosition= pos)); }

//Artillery::CanonRotate
void Artillery::CanonRotate(float ang) {
    _canonAngle = ang;
    GetSprite("canon")->SetRotation( -ang);
    if ( _cProjectile ) _cProjectile->Physic::SetPosition( GetSprite("canon")->TransformToGlobal( Vector2f(15, 0)) );
}

//Artillery::LoadProjectile
void Artillery::LoadProjectile(Projectile * p ) {
    _cProjectile = p;
    _cProjectile->Physic::SetPosition( GetSprite("canon")->TransformToGlobal( Vector2f(15, 0)) );
    _cProjectile->SetFree(false);
}

//Unit::Fire
void Artillery::Fire(float power) {
    if ( !_cProjectile ) return;
    float vel = 15 + power * 10;
    _cProjectile->AddVelocity( Vector2f(vel, vel+5));
    _cProjectile->SetFree(true);
    _cProjectile = 0x00;
}
