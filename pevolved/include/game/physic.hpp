#ifndef __physic__
#define __physic__

#include <SFML/Graphics.hpp>


class Physic {
    public:
        Physic();
        // getters&setters
        const sf::Vector2f getVel();
        const sf::Vector2f getSize();
        const sf::Vector2f getPos();
        const virtual sf::FloatRect getBB();


        void setVel( const sf::Vector2f & vel);
        virtual void setSize( const sf::Vector2f & size);
        virtual void setPos( const sf::Vector2f & pos);

        inline void setVel( float x, float y) { setVel( sf::Vector2f(x, y)); };
        inline void setSize( float x, float y) { setSize( sf::Vector2f(x, y)); };
        inline void setPos( float x, float y) { setPos( sf::Vector2f(x, y)); };

        enum geoStatus { geo_ground = 0, geo_air = 1};
        enum wallStatus { wall_none = 0, wall_left = 1, wall_right = 2, wall_both = 3};
        enum properties { property_fly = 1, property_projectile = 2, property_noclip = 4 };

        geoStatus myGeo;
        int myWall;

        int myProps;

    protected:
        // UPDATORS
        inline virtual void Moved() {};
        sf::Vector2f myPos, mySize, myVel;

};


#endif
