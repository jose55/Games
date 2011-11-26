#ifndef __gameCharacter__
#define __gameCharacter__

#include <SFML/Graphics.hpp>

#include "entity.hpp"

class Database;
class AnimatedSprite;
class Projectile;

class Character : public Entity {
    public:
        Character();
        virtual ~Character();
        virtual int Update();
        virtual void PreThink();

        // Character related
        virtual void moveRight();
        virtual void moveLeft();
        virtual void jump();
        virtual Projectile * fire();

        int myMoonJump;

    protected:
        int myAngle, myLockMove;
        Database * myDb;
        Projectile * myProjectile;

        virtual void moveGen();


};

#endif

