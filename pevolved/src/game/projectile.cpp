#include "projectile.hpp"
#include "database.hpp"
#include "animatedSprite.hpp"

using namespace sf;

Projectile::Projectile() {
    Database * db = Database::getDatabase();
    mySprite = new AnimatedSprite();
    mySprite->setResource( db->getImage("res/sprites/rockmanx.png"));
    mySprite->addFrame( IntRect(81, 75, 88, 80), animation_idle, 1 );
    myProps |= property_fly | property_projectile | property_noclip;
}

Projectile::Projectile( const Projectile & p, int angle) {
    mySprite = new AnimatedSprite(*(p.mySprite));
    myProps = p.myProps;
    myAngle = angle;
    mySize = mySprite->GetSize();
    myBB = Shape::Rectangle(0, 0, 8, 8, Color::Red);
    myExistence.Reset();
}

Projectile::~Projectile() {
}

int Projectile::Update() {
    myVel.x = 8*myAngle;
    myBB.SetX( getBB().Left+myVel.x);
    myBB.SetY( getBB().Top);
    if ( myExistence.GetElapsedTime() > 3.00 ) return 0;
    return 1;
}
