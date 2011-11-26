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
    registerKey( Key::Right );
    registerKey( Key::Left );
    registerKey( Key::Space );
    registerKey( Key::H );
    registerKey( Key::P );
    registerKey( Key::N );
    registerKey( Key::LControl );
    registerKey( Key::Back );

    // Registering physic vars
    myBound = FloatRect(0, -100, 9999, 9999);
    myBoundShape.AddPoint(myBound.Left, myBound.Top, Color::Blue);
    myBoundShape.AddPoint(myBound.Right, myBound.Top, Color::Blue);
    myBoundShape.AddPoint(myBound.Right, myBound.Bottom, Color::Blue);
    myBoundShape.AddPoint(myBound.Left, myBound.Bottom, Color::Blue);
    myBoundShape.SetBlendMode( Blend::Add );

    myDb = Database::getDatabase();

    // Variables
    myPhysicPause = 0;

    // Camera

    // HUD
    myLife.SetImage(*myDb->getImage("res/hud/life.png"));
    myLifeBar.SetImage( *myDb->getImage("res/hud/lifebar.png"));

    myLife.SetPosition(12, 32);
    myLifeBar.SetPosition(12, 32);

}


void Game::Init() {
    myView->SetFromRect( FloatRect(0, 0, 800, 600));
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

    if ( myKeys[Key::Right].down )
            myChar->moveRight();
    else  if ( myKeys[Key::Left].down ) myChar->moveLeft();

    else myChar->setVel( myChar->getVel().x/2.f, myChar->getVel().y );

    if ( myKeys[Key::Space].pressed )
        myChar->jump();

    if ( myKeys[Key::LControl].pressed ) {
        Projectile * p = myChar->fire();
        if ( p ) myEnts.push_back(p);
    }

    // Debug Controls
    if ( myKeys[Key::H].pressed ) myDb->debug = (myDb->debug+1)%2;
    if ( myKeys[Key::P].pressed ) myPhysicPause = (myPhysicPause+1)%2;

    // Physic Engine
    if ( !myPhysicPause ) UpdatePhysicEngine();
    else if ( myKeys[Key::N].pressed ) UpdatePhysicEngine();

    // Camera Update
    myView->SetCenter( myChar->getPos ());

    // Camera Bounds
    if ( myView->GetRect().Left < 0 ) myView->Move( -myView->GetRect().Left, 0);

    if ( myKeys[Key::Back].pressed ) {
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

        if ( e->myGeo != Physic::geo_ground && pos.y < bound.Bottom && !(e->myProps&Physic::property_fly) ) {
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

            if ( !(pos.x+size.x <= objBB.Left || pos.x >= objBB.Right) ) {
                if ( pos.y <= objBB.Top && bound.Bottom > objBB.Top ) bound.Bottom = objBB.Top;
                if ( pos.y-size.y >= objBB.Bottom && bound.Top < objBB.Bottom ) bound.Top = objBB.Bottom;
            }
            if ( !(pos.y-size.y+vel.y >= objBB.Bottom || pos.y+vel.y <= objBB.Top ) ) {
                if ( pos.x+size.x <= objBB.Left && bound.Right > objBB.Left ) bound.Right = objBB.Left;
                if ( pos.x >= objBB.Right && bound.Left < objBB.Right ) bound.Left = objBB.Right;
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
            if ( bb1.Left > bb2.Right || bb1.Right < bb2.Left || bb1.Top > bb2.Bottom || bb1.Bottom < bb2.Top ) continue;
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
            if ( pos.y + vel.y >= bound.Bottom ) {
                vel.y = bound.Bottom - pos.y;
                e->myGeo = Physic::geo_ground;
            }
            else e->myGeo = Physic::geo_air;


            // Upper bound
            if ( pos.y-size.y + vel.y < bound.Top ) vel.y = (pos.y-size.y) - bound.Top;

            // Right bound
            if ( pos.x+size.x+vel.x >= bound.Right ) {
                vel.x = bound.Right - (pos.x+size.x);
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
                myBound.Right != bound.Right ||
                myBound.Top != bound.Top ||
                myBound.Bottom != bound.Bottom) {
                myBound = bound;
                myBoundShape.SetPointPosition(0, myBound.Left, myBound.Top);
                myBoundShape.SetPointPosition(1, myBound.Right, myBound.Top);
                myBoundShape.SetPointPosition(2, myBound.Right, myBound.Bottom);
                myBoundShape.SetPointPosition(3, myBound.Left, myBound.Bottom);
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
