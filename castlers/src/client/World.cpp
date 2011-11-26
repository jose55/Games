#include "World.hpp"

#include <cstdio>


using sf::Vector2f;
using std::vector;

extern bool test;

void World::Update() { // PHYSIC HAPPENS HERE
    for(vector<Physic*>::iterator it = _objects.begin(); it != _objects.end(); ++it) {
        Physic * obj = (*it);
        if ( obj->Hit(0) || !obj->IsFree() ) continue; // Will be replaced
        bool freefall = false;
        Vector2f s = obj->GetVelocity();
        Vector2f tmp = Vector2f( (s.y < 0 ) ? obj->GetY() : obj->GetY() - s.y, (s.y > 0) ? obj->GetY(1) : obj->GetY(1) - s.y  );

        float maxY = GetYAtX(obj->GetX());
        float minY = -999;
        float maxX = 9999;
        float minX = 0;

        Physic * rObj = 0x00, * lObj = 0x00;
        vector<Physic*>::iterator iMem[2];

        for(vector<Physic*>::iterator it2 = _objects.begin(); it2 != _objects.end(); ++it2) {
            Physic * obj2 = (*it2);
            if ( obj == obj2 || obj2->Hit(0) ) continue;

            if ( obj->GetY(1) <= obj2->GetY() )
                if ( obj->ShareX(obj2) && obj2->GetY() < maxY ) maxY = obj2->GetY();
            if ( obj->GetY() >= obj2->GetY(1) )
                if ( obj->ShareX(obj2) && obj2->GetY(1) > minY ) minY = obj2->GetY(1);

            if ( obj->ShareY(obj2) ) {
                    if ( obj->GetX(1) <= obj2->GetX() && obj2->GetX() <= maxX ) {
                        maxX = obj2->GetX();
                        iMem[0] = it2;
                        rObj = obj2;
                    }

                    if ( obj->GetX() >= obj2->GetX(1) && obj2->GetX(1) >= minX ) {
                        minX = obj2->GetX(1);
                        iMem[1] = it2;
                        lObj = obj2;
                    }
            }
        }
        if ( maxY > obj->GetY(1) ) freefall = true;

        // Axe Horizontal
        if ( s.x != 0 ) {
            if ( obj->GetX(1) + s.x >= maxX)
                s.x = maxX - obj->GetX(1);
            else if ( obj->GetX() + s.x < minX )
                s.x = obj->GetX() - minX;

            obj->Move(s.x,  0);

            if ( !freefall )
                s = Vector2f( s.x/2, 0 );
        }
        // Axe Verticale
        if ( s.y > 0 ) {
            if ( obj->GetY() - s.y < minY ) s.y =  minY - obj->GetY();
            obj->Move(0, -s.y);
            s.y -= 0.5;
        }
        else if ( freefall ) {
            s.y -= 0.5;
            if ( s.y < -20 ) s.y = -20.0;
            if ( obj->GetY(1) - s.y > maxY ) {
                obj->Move(0, maxY - obj->GetY(1));
                s.y = 0;
            }
            else obj->Move(0, -s.y);
        }
        // IMPACT
        if ( rObj && Collides(obj, rObj) && obj->GetVelocity().x > 5 && rObj->IsBreakable()) {
            rObj->Hit(4);
            _cross.SetPosition( rObj->GetPosition() );
            _cross2.SetPosition( rObj->GetPosition() );
            for(vector<Physic*>::iterator it3 = _objects.begin(); it3 != _objects.end(); ++it3) {
                if ( Collides(&_cross, (*it3) ) || Collides(&_cross2, (*it3)) )
                    (*it3)->Hit(2);
            }
            //_objectsDead.push_back(iMem[0]);
            rObj = 0x00;
            s.x = 0; s.y = 0;
        }


        obj->AddVelocity( Vector2f(s.x, s.y) );


    }
    for(vector<vector<Physic*>::iterator>::iterator idel = _objectsDead.begin(); idel != _objectsDead.end();) {
        _objects.erase((*idel));
        _objectsDead.erase(idel);
        continue;
    }
}

World::World() {
    _cross.SetSize(28, 32*3 - 4);
    _cross2.SetSize(32*3 - 4, 28);
}

/* OBJECTS */
void World::MergeObjects(vector<Physic*> * nobjs) {
    for(vector<Physic*>::iterator it = nobjs->begin(); it != nobjs->end(); ++it)
        _objects.push_back((*it));
    printf("[World] %d new objects merged\n",nobjs->size());
}

void World::AddObject(Physic * obj) { _objects.push_back(obj); }

void World::DelObject(Physic * obj) {
    for(vector<Physic*>::iterator it = _objects.begin(); it != _objects.end(); ++it) {
        if ( obj == (*it) ) {
            _objects.erase(it);
            break;
        }
    }
};



float World::GetYAtX(float x) {
    return 600;
}

bool World::Collides(Physic * p1, Physic * p2) {
    if ( p1->GetX(1) < p2->GetX() || p1->GetX() > p2->GetX(1)
        || p1->GetY(1) < p2->GetY() || p1->GetY() > p2->GetY(1) ) return false;
        return  true;
}
