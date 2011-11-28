#ifndef __physPlatform__
#define __physPlatform__

#include <SFML/Graphics.hpp>

#include "physic.hpp"


class Platform : public Physic, public sf::Drawable {
    public:
        Platform();
        Platform( sf::Vector2f position, sf::Vector2f size );
        ~Platform();

        void Render( sf::RenderTarget&, sf::Renderer&) const;

        virtual void setSize( const sf::Vector2f & size);
        virtual void setPos( const sf::Vector2f & pos);
        virtual void setX( float x);
        virtual void setY( float y);

        // Editor
        const virtual sf::Vector2f getPosEditor();
        const virtual sf::Vector2f getSizeEditor();

        // Tmp
        void materialize();

    private:
        sf::Shape myShape;
        std::vector<sf::Sprite*>myForm;

        const static int blockSize;
};


#endif
