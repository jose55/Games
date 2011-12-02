#ifndef __screenGame__
#define wscreenGame__

#include <vector>

#include <addons/engine/screen.hpp>

class Area;
class Stage;
class Physic;
class Entity;
class Character;
class Database;
class Enemy;

class Game : public Screen {
    public:
        Game();
        virtual ~Game();
        int Update();
        //void Display();
        //void DisplayHUD();
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

	// Tmp
	sf::View myView;

};

#endif
