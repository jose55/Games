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
//===========================================================
namespace SGUI {
    class Widget;
    class Button;
    class GUI;

    enum widget_type { widget_button = 0, widget_label, widget_box };

    class Manager : public sf::Drawable  {
        public:
            Manager();
            ~Manager();
            void Update( const sf::Vector2f mouse = sf::Vector2f(0, 0));
            void Render( sf::RenderTarget& target) const;

            void addGUI( SGUI::GUI * g);

            bool onGUI();
            bool onWidget();

            SGUI::Widget * getWidget();
            SGUI::GUI * getGUI();

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
            void setImage( sf::Image * image);
            void setShape( const sf::Color col);

            void addWidget( SGUI::Widget * w, const sf::Vector2f & p = sf::Vector2f(0, 0));
            SGUI::Widget * getWidget( const sf::Vector2f &p);

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
            std::vector<SGUI::Widget*>myWidgets;
    };

    class Alert : public GUI {
        public:
            Alert();

            void setText( std::string text );
            void Display();

            virtual void Render( sf::RenderTarget& target) const;
            virtual void Update();

        private:
            sf::String myString;
            int myStatus;
            float myLength;
            sf::Clock myClock;
    };

    class Widget : public sf::Drawable {
        public:
            Widget();
            virtual ~Widget();

            virtual void setPosition( const sf::Vector2f & p) = 0;

            inline const sf::Vector2f getPosition() { return myPos; };
            inline const sf::Vector2f getSize() { return mySize; };

            virtual void Render( sf::RenderTarget & target) const = 0;

            inline virtual int getValue() { return myValue; };
            inline void setValue( int val ) { myValue = val; };

            inline int getType() { return myType; }

        protected:
            sf::Vector2f myPos, mySize;

            int myValue;

            widget_type myType;

            friend SGUI::Widget * SGUI::GUI::getWidget( const sf::Vector2f &p);


    };

    class Label : public Widget {
        public:
            Label();

            void setPosition( const sf::Vector2f & p);
            void Render( sf::RenderTarget & target) const;


            void setText( std::string text);

        private:
            sf::String myText;
    };

    class Button : public Widget {
        public:
            Button( int return_value );
            ~Button();

            void setPosition( float x, float y);
            void setPosition( const sf::Vector2f & p);


            void setImage( sf::Image * image);

            void Render( sf::RenderTarget & target) const;

        private:
            sf::Shape * myShape;
            sf::Sprite * mySprite;
    };

    class IntBox : public Widget {
        public:
            IntBox();
            void setPosition( const sf::Vector2f & p);
            void Render( sf::RenderTarget & target) const;

            void setValue( int val);

        private:
            sf::Shape myBox;
            sf::String myText;

    };
};

#endif
