#ifndef __CLASS_MAINMENU__
#define __CLASS_MAINMENU__

#include <SFML/Graphics.hpp>

#include "Mode.hpp"
#include "Windows.hpp"


class MainMenu : public Mode {
    public:
    MainMenu(sf::RenderWindow * win);
    bool Update();
    void Display();

    private:
        sf::Image _splashImage;
        sf::Sprite _splash;

        // tmp
        TWindow * _wintmp;
};

#endif
