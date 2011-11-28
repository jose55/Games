#ifndef __Database__
#define __Database__

#include <map>
#include <SFML/Graphics.hpp>

class Enemy;

class Database {
    public:
        static Database * getDatabase();
        void Init();

        sf::Texture* getImage(std::string);

        int debug;

        int getSmooth();
        void setSmooth(bool s);


        void addEnemy(Enemy * enemy, std::string name );
        Enemy * getEnemy(std::string name);

    private:
        // Construction
        Database();
        static Database * __instance;

        // Tables
        std::map<std::string, sf::Texture*>myImages;
        std::map<std::string, Enemy*>myEnemies;

        // Switches
        int mySmooth;

        // Generic Resources
        sf::Texture myImageNull;

};

#endif
