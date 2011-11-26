#ifndef __CLASS_CASTLE__
#define __CLASS_CASTLE__

#include <vector>

#include <SFML/Graphics.hpp>

#include "Drawer.hpp"
#include "Physic.hpp"
#include "Blueprints.hpp"
#include "common.hpp"

using namespace sf;
using std::vector;

class Block : public Physic{
    public:
    Block();
    Sprite * _spr;


    protected:
        void physicMoved();
};

/*
*   Castle Class
*
*   - version 0.0
*/
class Castle : public Drawer {
    public:
        Castle(cSprite * blocklist); // Ctor
        void Draw();

        vector<Physic*> * GetBlocks();
        void AddBlock(Block * b);
        void Move(float x, float y);
    private:
        vector<Physic*>_blocks;
        void Load();

        cSprite * _castleSpr;
};



#endif
