#ifndef __Widgets__
#define __Widgets__

#include <addons/gui/widget.hpp>

namespace SGUI {
   class Label : public Widget {
        public:
            Label();

            void setPosition( const sf::Vector2f & p);
            void Render( sf::RenderTarget&, sf::Renderer&) const;


            void setText( std::string text);

        private:
            sf::Text myText;
    };

    class Button : public Widget {
        public:
            Button( int return_value );
            ~Button();

            void setPosition( float x, float y);
            void setPosition( const sf::Vector2f & p);


            void setImage( sf::Texture*);

            void Render( sf::RenderTarget&, sf::Renderer&) const;

        private:
            sf::Shape * myShape;
            sf::Sprite * mySprite;
    };

    class IntBox : public Widget {
        public:
            IntBox();
            void setPosition( const sf::Vector2f & p);
            void Render( sf::RenderTarget&, sf::Renderer&) const;

            void setValue( int val);

        private:
            sf::Shape myBox;
            sf::Text myText;

    };
};
#endif
