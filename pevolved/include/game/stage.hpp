#ifndef __gameStage__
#define __gameStage__

#include <vector>

#include <SFML/Graphics.hpp>

class Physic;
class Platform;
class Enemy;

class Area : public sf::Drawable {
    public:
        Area();
        ~Area();
        std::vector<Platform*>myPlatforms;
        std::vector<Enemy*>myEnemies;

        void Render( sf::RenderTarget & target) const;

        const sf::Vector2f getSpawnPoint();

        void Clear();


    private:
        sf::Vector2f mySpawnPoint;
};

class Stage {
    public:
        Stage();
        ~Stage();
        Area * getArea( int i );

        void Save();
        void Load();

    private:
        std::vector<Area*>myAreas;

};

#endif
