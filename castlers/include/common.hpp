#ifndef __COMMON__
#define __COMMON__

#include <SFML/Graphics.hpp>

struct cSprite {
    sf::Sprite * spr;
    sf::Vector2f pos, size;
    sf::Shape _shape;
    int _id;
    cSprite * next, * previous;
};

void Snap(int x, int y, sf::Sprite * s);
#endif
