#ifndef __screenGame__
#define __screenGame__

#include <vector>

#include "screenManager.hpp"

class Area;
class Stage;
class Physic;
class Entity;
class Character;
class Database;
class Enemy;

class Game : public Screen {
    public:
        Game( sf::RenderWindow * win);
        virtual ~Game();
        void Update();
        void Display();
        void DisplayHUD();
        void Init();

    private:
        void UpdatePhysicEngine();

        Database * myDb;
        Stage * myStage;
        Area * myCurrentArea;
        Character * myChar;

        // Physic Engine
        std::vector<Physic*>myPhysics;
        std::vector<Entity*>myEnts;
        sf::FloatRect myBound;
        sf::Shape myBoundShape;
        int myPhysicPause;

        // HUD
        sf::Sprite myLifeBar;
        sf::Sprite myLife;

};

#endif
