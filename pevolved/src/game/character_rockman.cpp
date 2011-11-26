#include <iostream>

#include "character_rockman.hpp"
#include "animatedSprite.hpp"


cRockman::cRockman() {}

void cRockman::jump() {
    if ( myGeo == geo_ground ) {
        myVel.y = -10.f;
        mySprite->setCurrentSequence( animation_jump );
    }
    if ( myGeo == geo_air  && myWall != wall_none )  {
        myVel.y = -10.f;
        myVel.x = 6.f * -myAngle;
        myLockMove = 6;
        mySprite->setCurrentSequence( animation_jump );
    }
}

void cRockman::PreThink() {}

int cRockman::Update() {
    Character::Update();
    return 1;
}
