#include <iostream>

#include "game.hpp"
#include "game/database.hpp"
#include "game/physic.hpp"
#include "game/entity.hpp"
#include "game/stage.hpp"
#include "game/platform.hpp"
#include "game/character.hpp"
#include "game/character_rockman.hpp"
#include "game/enemy.hpp"
#include "game/projectile.hpp"


using namespace std;
using namespace sf;

Game::Game( sf::RenderWindow * win) : Screen(win) {
    // Load stage
    myStage = new Stage();
    myStage->Load();
    myCurrentArea = myStage->getArea(0);

    // Merging platforms
    for( vector<Platform*>::iterator it = myCurrentArea->myPlatforms.begin(); it != myCurrentArea->myPlatforms.end(); ++it)
        myPhysics.push_back(*it);

    // Loading Character
    myChar = new cRockman();
    myChar->setPos( myCurrentArea->getSpawnPoint() );
    myEnts.push_back(myChar);

    // Loading Enemies
    for( std::vector<Enemy*>::iterator it = myCurrentArea->myEnemies.begin(); it != myCurrentArea->myEnemies.end(); ++it)
        myEnts.push_back( *it);

    // Registering keys
    registerKey( Keyboard::Right );
    registerKey( Keyboard::Left );
    registerKey( Keyboard::Space );
    registerKey( Keyboard::H );
    registerKey( Keyboard::P );
    registerKey( Keyboard::N );
    registerKey( Keyboard::LControl );
    registerKey( Keyboard::Back );

    // Registering physic vars
    myBound = FloatRect(0, -100, 9999, 9999);
    myBoundShape.AddPoint(myBound.Left, myBound.Top, Color::Blue);
    myBoundShape.AddPoint(myBound.Width, myBound.Top, Color::Blue);
    myBoundShape.AddPoint(myBound.Width, myBound.Height, Color::Blue);
    myBoundShape.AddPoint(myBound.Left, myBound.Height, Color::Blue);
    myBoundShape.SetBlendMode( Blend::Add );

    myDb = Database::getDatabase();

    // Variables
    myPhysicPause = 0;

    // Camera

    // HUD
    myLife.SetTexture(*myDb->getImage("res/hud/life.png"));
    myLifeBar.SetTexture( *myDb->getImage("res/hud/lifebar.png"));

    myLife.SetPosition(12, 32);
    myLifeBar.SetPosition(12, 32);

}


void Game::Init() {
    myView->SetViewport( FloatRect(0, 0, 800, 600));
}

Game::~Game() {
    for( vector<Physic*>::iterator it = myPhysics.begin(); it != myPhysics.end(); ) {
        delete *it;
        myPhysics.erase(it);
    }

    for( vector<Entity*>::iterator it = myEnts.begin(); it != myEnts.end(); ) {
        delete *it;
        myEnts.erase(it);
    }

    if ( myStage ) delete myStage;

    myChar = 0x00;

}


void Game::Update() {
    myChar->PreThink();

    Vector2f vel = myChar->getVel();
    //if ( myMouseLeft ) cout << "Here:" << myMouse.x << "," << myMouse.y;

    if ( myKeys[Keyboard::W].down )
            myChar->moveRight();
    else  if ( myKeys[Keyboard::Left].down ) myChar->moveLeft();

    else myChar->setVel( myChar->getVel().x/2.f, myChar->getVel().y );

    if ( myKeys[Keyboard::Space].pressed )
        myChar->jump();

    if ( myKeys[Keyboard::LControl].pressed ) {
        Projectile * p = myChar->fire();
        if ( p ) myEnts.push_back(p);
    }

    // Debug Controls
    if ( myKeys[Keyboard::H].pressed ) myDb->debug = (myDb->debug+1)%2;
    if ( myKeys[Keyboard::P].pressed ) myPhysicPause = (myPhysicPause+1)%2;

    // Physic Engine
    if ( !myPhysicPause ) UpdatePhysicEngine();
    else if ( myKeys[Keyboard::N].pressed ) UpdatePhysicEngine();

    // Camera Update
    myView->SetCenter( myChar->getPos ());

    // Camera Bounds
    if ( myView->GetViewport().Left < 0 ) myView->Move( -myView->GetViewport().Left, 0);

    if ( myKeys[Keyboard::Back].pressed ) {
        myNextScreen = ScreenMenu;
    }
}

void Game::UpdatePhysicEngine() {
    for( vector<Entity*>::iterator it = myEnts.begin(); it != myEnts.end();) {
        Entity * e = *it;
        FloatRect bound=FloatRect(0, -100, 9999, 9999);

        int die = 0;

        if ( !e->Update() ) {
            delete e;
            myEnts.erase(it);
            continue;
        }

        sf::Vector2f pos = e->getPos();
        sf::Vector2f vel = e->getVel();
        sf::Vector2f size = e->getSize();
        pos.x -= size.x/2;

        //cout << "MyPos: " << pos.x << "," << pos.y << endl;
        e->myWall = Physic::wall_none;

        if ( e->myGeo != Physic::geo_ground && pos.y < bound.Height && !(e->myProps&Physic::property_fly) ) {
            if ( vel.y >= 0.f && vel.y < 8.f) { // Freefalling
                if ( vel.y < 1.f ) vel.y = 1.f;
                vel.y *= 1.75;
                if ( vel.y > 4.f ) vel.y = 4.f;
            }
            if ( vel.y < 0 ) { // jumpin'
                vel.y /= 1.2f;
                if ( vel.y >= -1 ) vel.y = 0;
            }
        }

        /*
        *   SOLIDS !!
        *
        */
        if ( !(e->myProps&Physic::property_noclip)) for( vector<Physic*>::iterator it2 = myPhysics.begin(); it2 != myPhysics.end(); ) {
            Physic * obj = *it2;
            FloatRect objBB = obj->getBB();

            if ( !(pos.x+size.x <= objBB.Left || pos.x >= objBB.Width) ) {
                if ( pos.y <= objBB.Top && bound.Height > objBB.Top ) bound.Height = objBB.Top;
                if ( pos.y-size.y >= objBB.Height && bound.Top < objBB.Height ) bound.Top = objBB.Height;
            }
            if ( !(pos.y-size.y+vel.y >= objBB.Height || pos.y+vel.y <= objBB.Top ) ) {
                if ( pos.x+size.x <= objBB.Left && bound.Width > objBB.Left ) bound.Width = objBB.Left;
                if ( pos.x >= objBB.Width && bound.Left < objBB.Width ) bound.Left = objBB.Width;
            }
            ++it2;
        }

        /*
        *   Entities
        *
        */
        for( vector<Entity*>::iterator it2 = myEnts.begin(); it2 != myEnts.end(); ++it2) {
            Entity * e2 = *it2;

            if ( e == e2 || (e2->myProps&Physic::property_projectile) ) continue;
            FloatRect bb1= e->getBB(), bb2 =e2->getBB();
            if ( bb1.Left > bb2.Width || bb1.Width < bb2.Left || bb1.Top > bb2.Height || bb1.Height < bb2.Top ) continue;
            // beyond that, entities collide

            if ( e->myOwner != e2->myOwner ) {
                if ( (e->myProps&Physic::property_projectile) && e2->hit( e->getDamage() ) ) {
                    die = 1;
                    break;
                }
            }
        }
        // Overlimit
        // Lower Bound
        if ( !(e->myProps&Physic::property_projectile) ) {
            if ( pos.y + vel.y >= bound.Height ) {
                vel.y = bound.Height - pos.y;
                e->myGeo = Physic::geo_ground;
            }
            else e->myGeo = Physic::geo_air;


            // Upper bound
            if ( pos.y-size.y + vel.y < bound.Top ) vel.y = (pos.y-size.y) - bound.Top;

            // Width bound
            if ( pos.x+size.x+vel.x >= bound.Width ) {
                vel.x = bound.Width - (pos.x+size.x);
                e->myWall |= Physic::wall_right;
            }

            if ( pos.x+vel.x <= bound.Left ) {
                vel.x = bound.Left - pos.x;
                e->myWall |= Physic::wall_left;
            }

        }

        e->setVel( vel  );
        e->Move( vel);

        if ( it == myEnts.begin() ) {
            if ( myBound.Left != bound.Left ||
                myBound.Width != bound.Width ||
                myBound.Top != bound.Top ||
                myBound.Height != bound.Height) {
                myBound = bound;
                myBoundShape.SetPointPosition(0, myBound.Left, myBound.Top);
                myBoundShape.SetPointPosition(1, myBound.Width, myBound.Top);
                myBoundShape.SetPointPosition(2, myBound.Width, myBound.Height);
                myBoundShape.SetPointPosition(3, myBound.Left, myBound.Height);
            }
        }

        if ( die ) {
            delete e;
            myEnts.erase(it);
        }
        else ++it;
    }


    //cout << "pos.x: " << pos.x << " - vel.x: " << (int)vel.x << "Boundary Left: " << bound.Left << endl;


}

void Game::Display() {
    if ( myCurrentArea ) Draw(myCurrentArea);
    for( std::vector<Entity*>::const_iterator it = myEnts.begin(); it != myEnts.end(); ++it)
        Draw(*it);

    //Draw(myChar);

    if ( myDb->debug ) Draw(&myBoundShape);
}

void Game::DisplayHUD() {
    Draw(&myLifeBar);
    Draw(&myLife);
}
