#include <iostream>

#include <game/entity.hpp>
#include <game/animatedSprite.hpp>
#include <game/database.hpp>

Entity::Entity() {
    mySprite = new AnimatedSprite();
    myBB.SetBlendMode( sf::Blend::Add );
    myLife = 1.f;
}

Entity::~Entity() {
    if ( mySprite ) delete mySprite;
}


void Entity::setPos( const sf::Vector2f & pos) {
    myPos = pos;
    mySprite->SetPosition(pos);
    myBB.SetPosition(pos);
}


void Entity::Move( const sf::Vector2f & dist) {
    myPos += dist;
    mySprite->Move(dist);
}

int Entity::Update() {
    myBB.SetPosition( myPos - sf::Vector2f(mySize.x/2, mySize.y) );
    return myLife > 0;
}

void Entity::Render(sf::RenderTarget& target, sf::Renderer& renderer) const {
    target.Draw(*mySprite);
    if ( Database::getDatabase()->debug ) target.Draw(myBB);
}

const sf::FloatRect Entity::getBB() {
    return sf::FloatRect(myPos.x-mySize.x/2, myPos.y-mySize.y, myPos.x+mySize.x/2, myPos.y);
}


int Entity::hit( int damage ) {
    myLife -= damage;
    return myLife <= 0;
}

float Entity::getLife() { return myLife; }
