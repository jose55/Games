#include "Projectiles.hpp"

using namespace sf;

Projectile::Projectile() : Physic( Physic::hbSphere ) {
    Image * t = new Image();
    t->LoadFromFile("res/projectile.bmp");
    t->SetSmooth(false);
    t->CreateMaskFromColor(Color(255, 0, 255));
    p.SetImage(*t);
    SetSize( t->GetWidth(), t->GetHeight());
    p.SetCenter( GetSize().x/2, GetSize().y/2);

}

void Projectile::Render(RenderTarget& target) const {
    target.Draw(p);
}

void Projectile::physicMoved() { p.SetPosition(Physic::GetPosition()); }
