#ifndef __CLASS_DRAWER__
#define __CLASS_DRAWER__

#include <vector>

#include <SFML/Graphics.hpp>

/*
*   Drawer Class
*
*   - version 0.1
*   Optimize all classes that need to draw onto screen
*/
class Drawer {
    public:
        void SetWindow(sf::RenderWindow * win);
        virtual void DrawBack(); // Background Drawing (for 3D Effect)
        virtual void Draw() = 0;

        ~Drawer();


    protected:
        sf::Sprite * GetSprite(std::string name);
        void AddSprite(std::string name, sf::Sprite * s);
        sf::RenderWindow * _win;

    private:
        std::map<std::string, sf::Sprite*>_sprites;
};
#endif
