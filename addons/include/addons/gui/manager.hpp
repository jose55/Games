#ifndef __GUI_MANAGER__
#define __GUI_MANAGER__


//===========================================================
//  Salepate GUI
//  v0.3
//  uses vector,SFML
//  v0.3:
//  - removed Button text functions
//  - added Widget
//  v0.2:
//  - added Button::setImage()
//  - improved button creation
#include <vector>
#include <SFML/Graphics.hpp>
#include <addons/gui/widget.hpp>
//===========================================================
namespace SGUI {
    class Button;
    class GUI;

    class Manager : public sf::Drawable  {
        public:
            Manager();
            ~Manager();
            void Update( const sf::Vector2f mouse = sf::Vector2f(0, 0));
            void Render( sf::RenderTarget& target) const;

            void addGUI( SGUI::GUI * g);

            bool onGUI();
            bool onWidget();

            Widget * getWidget();
            GUI * getGUI();

        private:

            std::vector<SGUI::GUI*>myGUIs;

            sf::Shape myShape;
            sf::Vector2f myMouse;
            Widget * myWidget;
    };


    class GUI : public sf::Drawable {
        public:
            GUI();
            ~GUI();

            void setPosition( float x, float y);
            void setPosition( const sf::Vector2f & p);

            void setSize( float x, float y);
            void setImage( sf::Texture*);
            void setShape( const sf::Color col);

            void addWidget( Widget * w, const sf::Vector2f & p = sf::Vector2f(0, 0));
            Widget * getWidget( const sf::Vector2f &p);

            virtual void Render( sf::RenderTarget& target) const;

            const sf::Vector2f getPosition();
            const sf::Vector2f getSize();

            inline virtual void Update() {}

        protected:
            sf::Vector2f myPos, mySize;
            friend SGUI::GUI * SGUI::Manager::getGUI();

            sf::Shape * myShape;

        private:
            sf::Sprite mySprite;
            std::vector<Widget*>myWidgets;

    };


    class Alert : public GUI {
        public:
            Alert();

            void setText( std::string text );
            void Display();

            virtual void Render( sf::RenderTarget& target) const;
            virtual void Update();

        private:
            sf::Text myString;
            int myStatus;
            float myLength;
            sf::Clock myClock;
    };
};
#endif
