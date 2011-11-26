#ifndef __BLUEPRINTS__
#define __BLUEPRINTS__

#include <vector>

#include <SFML/System.hpp>

class Blueprint {
    public:
    int angle, flip;
};

class BpBlock : public Blueprint {
    public:
        sf::Vector2i position;
        int id;
};

class BpCastle {
    public:
        std::vector<BpBlock*>_blocks;
};

#endif
