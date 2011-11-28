#include <iostream>

#include <game/enemy.hpp>
#include <game/animatedSprite.hpp>
#include <game/database.hpp>


using namespace sf;

Enemy::Enemy() {
    mySprite->setResource( Database::getDatabase()->getImage("res/sprites/rockmanx_enemies.png") );
    mySprite->addFrame( IntRect(10, 301, 41, 337), animation_idle, 2);
    mySize.x=31;
    mySize.y=36;
    myProps = 0;
    myOwner = 2;
    myLife = 1;
}

Enemy::Enemy( const Enemy & e) {
    mySprite = new AnimatedSprite(*(e.mySprite));
    mySize = e.mySize;
    myProps = e.myProps;
    myBB = Shape::Rectangle(0, 0, mySize.x, mySize.y, Color::Magenta);
}



void Enemy::Move( const sf::Vector2f & dist) {
    myPos += dist;
    mySprite->Move(dist);
}
