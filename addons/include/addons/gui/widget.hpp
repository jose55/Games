#ifndef __SGEAddonGUIWidget__
#define __SGEAddonGUIWidget__

#include <SFML/Graphics.hpp>

namespace SGUI {
	enum widget_type { widget_button = 0, widget_label, widget_box };
	class Widget : public sf::Drawable {
		public:

		    Widget();
		    virtual ~Widget();

		    virtual void setPosition( const sf::Vector2f & p) = 0;

		    inline const sf::Vector2f& getPosition() { return myPos; }
		    inline const sf::Vector2f& getSize() { return mySize; }

		    virtual void Render( sf::RenderTarget&, sf::Renderer&) const = 0;

		    inline virtual int getValue() { return myValue; };
		    inline void setValue( int val ) { myValue = val; };

		    inline int getType() { return myType; }

		protected:
		    sf::Vector2f myPos, mySize;

		    int myValue;

		    widget_type myType;
	};

};

#endif
