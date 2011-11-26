#ifndef __CLASS_GAME__
#define __CLASS_GAME__

#include <vector>

#include <SFML/Graphics.hpp>


#include "Mode.hpp"

// Game Includes
#include "Drawer.hpp"
#include "Scene.hpp"
#include "Castle.hpp"
#include "Artillery.hpp"
#include "Database.hpp"
#include "MotionControl.hpp"
#include "World.hpp"
#include "HUD.hpp"

class Game : public Mode {
    public:
        Game(sf::RenderWindow * win, Database * db); // Basic Constructor
        ~Game();
        bool Update();
        void Display();

    protected:
        void CameraMove(float x);

    private:
        /* camera set */
        sf::View _camera;
        float _baseZoom;
        float _baseCameraX, _baseMoveX;

        std::vector<Drawer*>_drawers;
        MotionSystem::MotionControl * _motions;

        /* game set */
        World * _world;
        Scene _scene;
        Castle * _castle;
        std::vector<Unit*>_units;
        std::vector<Projectile*>_projectiles;

        Unit * _cUnit;

        // in-dev
        HUD_Manager::HUD _HUD;
        int _idArrow;


};

#endif
