#include <iostream>

#include <game/platform.hpp>
#include <game/database.hpp>


using namespace sf;
using namespace std;

const int Platform::blockSize = 32;

Platform::Platform() {
    mySize = Vector2f(blockSize, blockSize);
    myPos  = Vector2f(0, 0);

    myShape = Shape::Rectangle(myPos.x, myPos.y, mySize.x, mySize.y, Color::Red);
}

Platform::Platform( Vector2f position, Vector2f size ) {
    position.x *= blockSize;
    position.y *= blockSize;

    size.x *= blockSize;
    size.y *= blockSize;

    myPos = position;
    mySize = size;

    myShape = Shape::Rectangle( myPos.x, myPos.y, (myPos+mySize).x, (myPos+mySize).y, Color::Red);


}

Platform::~Platform() {
    if ( myForm.size() ) for( vector<Sprite*>::iterator it = myForm.begin(); it != myForm.end();) {
        delete *it;
        myForm.erase(it);
    }
}

void Platform::Render( sf::RenderTarget& target, sf::Renderer& renderer ) const {
    if ( myForm.size() > 0 ) {
        for( vector<Sprite*>::const_iterator it = myForm.begin(); it != myForm.end(); ++it)
            target.Draw(**it);
    }
    else target.Draw(myShape);
}

void Platform::setSize( const sf::Vector2f & size) {
    mySize = size;
    myShape.SetPointPosition(1, size.x, 0);
    myShape.SetPointPosition(2, size.x, size.y);
    myShape.SetPointPosition(3, 0, size.y);
}

void Platform::setPos( const sf::Vector2f & pos) {
    myPos = pos;
    myShape.SetPosition(pos);
}

void Platform::setX( float x) {
    myPos.x = x;
    myShape.SetX(x);
}
void Platform::setY( float y) {
    myPos.y = y;
    myShape.SetY(y);
}

const sf::Vector2f Platform::getPosEditor() {
    return sf::Vector2f( (int)myPos.x/32, (int)myPos.y/32);
}
const sf::Vector2f Platform::getSizeEditor() {
    return sf::Vector2f( (int)mySize.x/32, (int)mySize.y/32);
}

void Platform::materialize() {
    sf::Texture * blockImg = Database::getDatabase()->getImage("res/world/block.png");
    int size_x = (int)mySize.x/blockSize;
    int size_y = (int)mySize.y/blockSize;
    for( int i = 0; i < size_x; ++i)
        for(int j = 0; j < size_y; ++j) {
            Sprite * spr = new Sprite();
            spr->SetTexture(*blockImg);
            spr->SetPosition(myPos + Vector2f(i*blockSize, j*blockSize));
            myForm.push_back(spr);
        }
}
