#ifndef _CLASS_WORLD__
#define _CLASS_WORLD__

#include <vector>

#include "Physic.hpp"
#include "Projectiles.hpp"


class World {
    public:
    World();
    void Update();

    // Setters

    void MergeObjects(std::vector<Physic*> * nobjs);
    void AddObject(Physic * obj);
    void DelObject(Physic * obj);


    //private:
        std::vector<Physic*>_objects;
        std::vector<std::vector<Physic*>::iterator>_objectsDead;

        float GetYAtX(float x);
        bool Collides(Physic * p1, Physic * p2);

        // CROSS IMPACT
        Physic _cross, _cross2;

};


#endif
