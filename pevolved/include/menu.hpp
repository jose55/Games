#ifndef __screenMenu__
#define __screenMenu__

#include "screenManager.hpp"

class Database;

class Menu : public Screen {
    public:
        Menu( sf::RenderWindow * win);

        //inherit from Screen
        void Display();
        void Update();

    private:
        sf::Text myTextMenu;
        sf::Font myTextFont;
        Database * myDb;

        void createText();
};


#endif
