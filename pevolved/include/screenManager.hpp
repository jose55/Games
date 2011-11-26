#ifndef __screenManager__
#define __screenManager__

#include <SFML/Graphics.hpp>

class Screen {
    public:
        Screen( sf::RenderWindow * win);
        virtual ~Screen();
        virtual void Display() = 0;
        inline virtual void DisplayHUD() {};
        inline virtual void Init() {};

        void keyDown( int keyCode );
        void updateMousePos( const sf::Vector2f pos);
        void updateMouseClick();
        void releaseMouseClick();

        void updateScreen();

        int nextScreen();

        enum screens { ScreenMenu = 1, ScreenGame, ScreenEditor };

        //
        void setView( sf::View * view);

    protected:
        virtual void Update() = 0;

        void Draw( sf::Drawable * o);

        struct key {
            int pressed, down;
        };

        std::map<int, key>myKeys;
        void registerKey( int keyCode );
        sf::Vector2f myMouse;
        int myMouseLeft, myMouseLeftDown;

        int myNextScreen;

        sf::View * myView;

    private:
        sf::RenderWindow * myWin;


};

#endif

