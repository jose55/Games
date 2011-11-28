//#include <iostream>


#include <game/character.hpp>
#include <game/database.hpp>
#include <game/animatedSprite.hpp>
#include <game/projectile.hpp>


//using std::cout;
//using std::endl;
using namespace sf;

/*
*   Class Character
*   self-explanatory
*   Playable unit
*/

// Constructor
Character::Character() {

    myDb = Database::getDatabase();

    mySprite->setResource(myDb->getImage("res/sprites/rockmanx.png"));
    mySprite->addFrame( IntRect(7, 62, 36, 95), animation_idle  );

    // Walk sequence
    mySprite->addFrame( IntRect(6,   159, 35, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(40,  159, 59, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(64,  158, 86, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(93,  159, 124, 192), animation_walk, 2);
    mySprite->addFrame( IntRect(131, 160, 164, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(174, 160, 199, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(207, 159, 228, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(233, 158, 257, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(264, 158, 293, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(299, 159, 332, 192), animation_walk, 2 );
    mySprite->addFrame( IntRect(339, 159, 367, 192), animation_walk, 2 );

    // Wall Slide
    mySprite->addFrame( IntRect(3, 352, 27, 393), animation_custom1, 2);
    mySprite->addFrame( IntRect(37, 350, 63, 392), animation_custom1, 2);
    mySprite->addFrame( IntRect(75, 347, 102, 388), animation_custom1, 2);
    mySprite->setSequenceLoopStart(animation_custom1, 2);

    // Jump
    mySprite->addFrame( IntRect(11, 111, 34, 147), animation_jump, 2);
    mySprite->addFrame( IntRect(41, 107, 55, 147), animation_jump, 2);
    mySprite->addFrame( IntRect(63, 102, 81, 147), animation_jump, 2);
    mySprite->addFrame( IntRect(88, 104, 110, 144), animation_jump, 2);
    mySprite->addFrame( IntRect(118, 105, 144, 146), animation_jump, 2);
    mySprite->addFrame( IntRect(151, 108, 174, 145), animation_jump, 2);
    mySprite->setSequenceLoopStart(animation_jump, 5);


    setSize( mySprite->GetSize());

    myMoonJump = 0;

    myBB = Shape::Rectangle(0, 0, mySize.x, mySize.y, Color(100, 200, 100));

    // Testing
    myLockMove = 0;
    myAngle = 1;
    myProjectile = new Projectile();
    myProps = 0;
    myOwner = 1;
    myLife = 1;

}

Character::~Character() {}

// PreThink
void Character::PreThink() {

}

// Updater
int Character::Update() {
    if ( myLockMove > 0 ) --myLockMove;
    if ( myGeo == Physic::geo_ground && abs(myVel.x) < 0.5 && myVel.y >= 0 ) mySprite->setCurrentSequence(animation_idle);

    myBB.SetX( getBB().Left+myVel.x );
    myBB.SetY( getBB().Top+myVel.y );

    // Animation check
    if ( mySprite->getCurrentSequence() == animation_walk && myGeo == geo_air ) mySprite->setCurrentSequence( animation_idle );
    if ( mySprite->getCurrentSequence() == animation_custom1 && myWall == wall_none ) mySprite->setCurrentSequence( animation_idle );

    mySprite->Update();

    return 1;
}


/*
*   Character related methods
*
*
*/
void Character::moveGen() {
    if ( myLockMove > 0) return;
    wallStatus w = Physic::wall_right;
    if ( myAngle != 1 ) {
        myAngle = -1; // prevent exploits
        w = Physic::wall_left;
    }

    if ( (myWall&w) && myVel.y > 0 ) {
            myVel.y = 1; // slow down
            myMoonJump = 0; // give back moon jump
            mySprite->setCurrentSequence( animation_custom1 );
    }
    if ( myGeo == Physic::geo_ground ) {
         myVel.x = 4.f*myAngle;
         mySprite->setCurrentSequence( animation_walk );
    }
    if ( myGeo == Physic::geo_air ) {
        if ( myVel.x * myAngle < 0 ) myVel.x *= -1;
        if ( abs(myVel.x) < 4.f ) myVel.x += 0.3*myAngle;
        if ( abs(myVel.x) > 4.f ) {
            if ( myVel.x > 0 ) --myVel.x;
            else ++myVel.x;
        }
    }
}
void Character::moveRight() { mySprite->FlipX(false); myAngle = 1; moveGen(); }
void Character::moveLeft() { mySprite->FlipX(true); myAngle = -1; moveGen(); }

void Character::jump() {
    if ( myGeo == Physic::geo_ground ) {
        myMoonJump = 0;
        myVel.y = -10;
        mySprite->setCurrentSequence(animation_jump);
    }
    if ( myGeo == Physic::geo_air && myMoonJump == 0) {
        myMoonJump = 1;
        myVel.y = -10.f;

        if ( myVel.x > 0 && (myWall&Physic::wall_right)) {
            myVel.x = -6.f;
            myLockMove = 6;
            myMoonJump = 0;
        }
        else if ( myVel.x < 0 && (myWall&Physic::wall_left)) {
            myVel.x = 6.f;
            myLockMove = 6;
            myMoonJump = 0;
        }
        mySprite->setCurrentSequence(animation_jump, 1);
    }
}

Projectile * Character::fire() {
    int angle = myAngle;
    if ( myWall ) angle = -angle;
    Projectile * p = new Projectile(*myProjectile, angle);
    p->setPos( sf::Vector2f( myPos.x+mySize.x/2*angle, myPos.y-mySize.x/2));
    p->myOwner = 1;
    return p;
}
