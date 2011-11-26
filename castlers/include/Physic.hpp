#ifndef __CLASS_PHYSIC__
#define __CLASS_PHYSIC__

#include <SFML/Graphics.hpp>
#include <cstdio>

#include "common.hpp"
#include "HitBox.hpp"

class Physic {
    public:
        Physic(int h = hbBB );
        //
        void Move(float x, float y);

        void Snap(int x, int y);

        sf::Shape * DrawHitBox();

        // Getters
        inline sf::Vector2f GetPosition() { return _position; };
        inline int GetAngle() { return _angle; };

        sf::Vector2f GetSize();
        float GetY(int point = 0);
        float GetX(int point = 0);

        // Setters
        void SetPosition(sf::Vector2f pos);
        void SetPosition(float x, float y);
        void SetX(float x);
        void SetY(float y);
        void SetSize(sf::Vector2f size);
        void SetSize(float x, float y);
        void SetAngle(int a);

        // V0.2
        void AddVelocity(sf::Vector2f v);
        sf::Vector2f GetVelocity();
        void SetBreakable(bool v);
        bool IsBreakable();
        void SetFree(bool v);
        bool IsFree();
        bool Hit(int d);

        // V0.3
        enum hbtype { hbBB = 0, hbSphere = 1 };
        int _hType;
        HitBox * _hb;
        bool ShareX(Physic * obj);
        bool ShareY(Physic * obj);

    protected:
        virtual void physicMoved();

    private:
        sf::Vector2f _position, _speed, _size;
        sf::Shape _hitbox;
        int _angle;

        // v2.0
        bool _isBreakable, _isFree;
        int _hp;
};

#endif
