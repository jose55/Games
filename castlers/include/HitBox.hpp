#ifndef __CLASS_HITBOX__
#define __CLASS_HITBOX__

#include <SFML/System.hpp>

class HitBox {
    public:
        enum point { mid = 2, p2 = 1};
        virtual float GetY(int p = 0) = 0;
        virtual float GetX(int p = 0) = 0;

    protected:
        sf::Vector2f _mySize;
};

class HBBB : public HitBox {
    public:
        HBBB( sf::Vector2f size);
        float GetY(int p = 0);
        float GetX(int p = 0);

        inline sf::Vector2f GetSize() const { return _mySize; };
};

class HBSphere : public HitBox {
    public:
        HBSphere(float size);
        float GetY(int p = 0);
        float GetX(int p = 0);

        inline float GetSize() const { return _mySize.x; };
};


#endif
