#ifndef __charRockman__
#define __charRockman__

#include "character.hpp"

class cRockman : public Character {
    public:
        cRockman();
        void jump();
        void PreThink();
        int Update();

    private:
};


#endif
