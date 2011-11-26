#include <fstream>
#include <iostream>

#include "stage.hpp"
#include "platform.hpp"
#include "enemy.hpp"
#include "database.hpp"

using namespace sf;
using namespace std;

//==================================================================================================
//=  Class Area
//=
//=
//=
//==================================================================================================
Area::Area() {
    mySpawnPoint = Vector2f(0, 32*4);
}

Area::~Area() {}

void Area::Render( sf::RenderTarget & target) const {
    for( std::vector<Platform*>::const_iterator it = myPlatforms.begin(); it != myPlatforms.end(); ++it)
        target.Draw(**it);
}

void Area::Clear() {
    for( vector<Platform*>::iterator it = myPlatforms.begin(); it != myPlatforms.end(); ) {
        delete (*it);
        myPlatforms.erase(it);
    }

    for( vector<Enemy*>::iterator it = myEnemies.begin(); it != myEnemies.end(); ) {
        delete (*it);
        myEnemies.erase(it);
    }
}

const sf::Vector2f Area::getSpawnPoint() { return mySpawnPoint; }


//==================================================================================================
//=  Class Stage
//=
//=
//=
//==================================================================================================
Stage::Stage() {
    Area * a = new Area();
    myAreas.push_back(a);
}


Stage::~Stage() {
    delete myAreas[0];
}

Area * Stage::getArea( int i ) {
    return myAreas[i];
}


void Stage::Save() {
    string path = "data/stage/stage.ped";
    std::ofstream f (path.c_str(), ios::out | ios::binary);

    Area * a = myAreas[0];

    int platformCount = a->myPlatforms.size();
    int enemyCount = a->myEnemies.size();


    f.write( (const char*) &platformCount, sizeof(int));
    f.write( (const char*) &enemyCount, sizeof(int));

    for(vector<Platform*>::iterator it = a->myPlatforms.begin(); it != a->myPlatforms.end(); ++it) {
        Platform * p = *it;
        //std::cout << p->getPos().x << "," << p->getPos().y << " - " << p->getSize().x << "," << p->getSize().y << std::endl;

        f.write( (const char*) &(p->getPosEditor().x), sizeof(float) );
        f.write( (const char*) &(p->getPosEditor().y), sizeof(float) );
        f.write( (const char*) &(p->getSizeEditor().x), sizeof(float) );
        f.write( (const char*) &(p->getSizeEditor().y), sizeof(float) );
    }

    for(vector<Enemy*>::iterator it = a->myEnemies.begin(); it != a->myEnemies.end(); ++it) {
        Enemy * p = *it;
        f.write( (const char*) &(p->getPos().x), sizeof(float) );
        f.write( (const char*) &(p->getPos().y), sizeof(float) );
    }

    f.close();
}

void Stage::Load() {
    string path = "data/stage/stage.ped";
    std::fstream f (path.c_str(), ios::in | ios::binary);

    if ( !f.is_open() ) return;

    float buffer;
    int state = 0, i;
    int platformCount, enemyCount;
    sf::Vector2f pos, size;
    Enemy * enemy = Database::getDatabase()->getEnemy("basic");


    myAreas[0]->Clear();


    // Reading Sizes
    f.read( (char*) &platformCount, sizeof(int) );
    f.read( (char*) &enemyCount, sizeof(int) );

    i = 0;
    // Platforms
    while(1) {
        if ( i >= platformCount ) break;
        f.read( (char*) &(buffer), sizeof(float));
        switch(state) {
            default:{
                state = 0;
                pos.x = buffer;
                break;
            }
            case 1: pos.y = buffer; break;
            case 2: size.x = buffer; break;
            case 3: {
                size.y = buffer;
                Platform * p = new Platform(pos, size);
                p->materialize();
                myAreas[0]->myPlatforms.push_back(p);
                ++i;
                break;
            }
        }
        ++state;
    }

    // Enemies
    i = 0;
    while(1) {
        if ( i >= enemyCount) break;
        f.read( (char*)&(pos.x), sizeof(float));
        f.read( (char*)&(pos.y), sizeof(float));
        Enemy * e = new Enemy( *enemy );
        e->setPos(pos);
        myAreas[0]->myEnemies.push_back(e);
        ++i;
    }
    f.close();
}
