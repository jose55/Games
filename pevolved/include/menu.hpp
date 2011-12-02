#ifndef __screenMenu__
#define __screenMenu__

#include <addons/engine/screen.hpp>

class Database;

class Menu : public Screen {
    public:
        Menu();

        //inherit from Screen
        void Display();
        int Update();
	inline void GetEvent(sf::Event&) {}

    private:
        sf::Text myTextMenu;
        sf::Font myTextFont;
        Database * myDb;

        void createText();
};


#endif
