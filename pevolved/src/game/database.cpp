#include <iostream>


#include <game/database.hpp>
#include <game/enemy.hpp>

using namespace std;

Database * Database::__instance = 0x00;

Database * Database::getDatabase() {
    if ( !__instance ) __instance = new Database();
    return __instance;
}

Database::Database() {
    debug = 0;
    mySmooth = 1;

    myImageNull.LoadFromFile("res/generic/null.png");

}

void Database::Init() {
    Enemy * e = new Enemy();
    addEnemy(e, "basic");
}


void Database::addEnemy( Enemy * e, std::string name) {
    map<std::string, Enemy*>::iterator it = myEnemies.find(name);
    if ( it == myEnemies.end() ) {
        myEnemies.insert( make_pair(name, e));
    }
}

Enemy * Database::getEnemy( std::string name ) {
    map<std::string, Enemy*>::iterator it = myEnemies.find(name);
    if ( it == myEnemies.end() ) return 0x00;
    return it->second;
}


sf::Texture * Database::getImage( std::string path ) {
    std::map<std::string, sf::Texture*>::iterator it;

    if ( (it = myImages.find(path)) == myImages.end() ) {
        sf::Texture * img = new sf::Texture();
        if ( !img->LoadFromFile(path) ) {
            delete img;
            img = &myImageNull;
        }
        else {
            img->SetSmooth(mySmooth);
            myImages.insert( std::make_pair(path, img));
        }
        return img;
    }
    return it->second;
}


int Database::getSmooth() { return mySmooth; }
void Database::setSmooth(bool s) {
    mySmooth = s;
    for( map<string, sf::Texture*>::iterator it = myImages.begin(); it != myImages.end(); ++it)
        it->second->SetSmooth(s);
}
