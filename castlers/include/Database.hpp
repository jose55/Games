#ifndef __CLASS_DB__
#define __CLASS_DB__

#include <vector>

#include <SFML/Graphics.hpp>

#include "Castle.hpp"
#include "Projectiles.hpp"
#include "Artillery.hpp"
#include "MotionControl.hpp"
#include "common.hpp"

#define ITEM_TYPE_UNIT 0
#define ITEM_TYPE_BLOCK 1
/*
*   Database Class
*
*   - version 0.0
*/
class Database {
    public:
        Database();
        ~Database();
        sf::Image * RegisterImage(std::string path);
        Drawer * RegisterItem(int itype, int etype, const char name[]);
        Artillery * GetArtillery(const char name[]);
        cSprite * GetCastleSprites();
        MotionSystem::Motion * GetMotion(std::string name);
    private:
        std::map<std::string, Artillery*>_dbArtillery;
        std::map<std::string, sf::Image*>_dbImage;
        std::map<std::string, MotionSystem::Motion*>_dbMotions;
        cSprite * _castleSpr;
};


#endif
