#include <iostream>

#include <addons/gui/manager.hpp>
#include <addons/gui/widget.hpp>

using namespace SGUI;
using namespace std;


/*
*   SGUI::GUI
*
*/
GUI::GUI() {
    myShape = 0x00;

}

GUI::~GUI() {
    for( std::vector<Widget*>::iterator it = myWidgets.begin(); it != myWidgets.end();) {
        delete *it;
        myWidgets.erase(it);
    }
}

// SGUI::GUI::setPosition( float x, float y)
void GUI::setPosition( float x, float y) {
    myPos = sf::Vector2f(x, y);
    mySprite.SetPosition(myPos);
    if ( myShape ) myShape->SetPosition(x, y);
}

void GUI::setPosition( const sf::Vector2f & p) { setPosition(p.x, p.y); }

// SGUI::GUI::setSize( float x, float y)
void GUI::setSize( float x, float y) {
    mySize = sf::Vector2f(x, y);
    mySprite.SetSubRect( sf::IntRect(0, 0, x, y));
}

// SGUI::GUI::setImage( sf::Texture*)
void GUI::setImage( sf::Texture * texture) {
    mySprite.SetTexture( *texture );
    mySprite.SetPosition(myPos);
    mySize = sf::Vector2f( texture->GetWidth(), texture->GetHeight());
}

// SGUI::GUI::setShape( sf::Color col)
void GUI::setShape( sf::Color col) {
    if ( !myShape ) {
        myShape = new sf::Shape;

	sf::FloatRect tmp(myPos.x, myPos.y, myPos.x+mySize.x, myPos.y+mySize.y);
        *myShape = sf::Shape::Rectangle(tmp, sf::Color::White);
    }
    myShape->SetColor(col);
}

// SGUI::GUI::addWidget( SGUI::Widget * widget, const sf::Vector2f & p)
void GUI::addWidget( SGUI::Widget * widget, const sf::Vector2f & p) {
    widget->setPosition( myPos + p);
    myWidgets.push_back(widget);
}


// SGUI::GUI::getButton( const sf::Vector2f & p)
Widget * GUI::getWidget(const sf::Vector2f &p) {
    for( std::vector<Widget*>::iterator it = myWidgets.begin(); it != myWidgets.end(); ++it) {
        Widget * w = *it;
        if ( p.x >= w->getPosition().x &&
             p.x <= w->getPosition().x + w->getSize().x &&
             p.y >= w->getPosition().y &&
             p.y <= w->getPosition().y + w->getSize().y) return w;
    }
    return 0;
}


// SGUI::GUI::Render( sf::RenderTarget & target) const;
void GUI::Render( sf::RenderTarget& target) const {
    target.Draw(mySprite);
    if ( myShape ) target.Draw(*myShape);
    for( std::vector<Widget*>::const_iterator it = myWidgets.begin(); it != myWidgets.end(); ++it)
        target.Draw(**it);
}

// SGUI::GUI::getPosition()
const sf::Vector2f GUI::getPosition() { return myPos; }

// SGUI::GUI::getSize()
const sf::Vector2f GUI::getSize() { return mySize; }


// SGUI::Alert::Alert()
Alert::Alert() {
    myShape = new sf::Shape();
    *myShape = sf::Shape::Rectangle(0, 0, 100, 100, sf::Color(200, 0, 0, 70) );
    myLength = 0.f;
    myStatus = 0;
}

void Alert::setText( std::string text) {
    myString.SetString(text);
    myString.SetPosition(myPos+sf::Vector2f(2, 0));
    myShape->SetPointPosition(1, myString.GetRect().Width-myString.GetRect().Left+4, 0);
    myShape->SetPointPosition(2, myString.GetRect().Width-myString.GetRect().Left+4, myString.GetRect().Height);
    myShape->SetPointPosition(3, 0, myString.GetRect().Height);
}

void Alert::Display() {
    myClock.Reset();
    myStatus = 1;
}

void Alert::Render( sf::RenderTarget & target) const {
    if ( myStatus ) {
        target.Draw(*myShape);
        target.Draw(myString);
    }
}

void Alert::Update() {
    if ( myClock.GetElapsedTime() > 3.f ) myStatus = 0;

}
/*
*   SGUI::Manager
*
*/
Manager::Manager() {
    myWidget = 0x00;
    myShape = sf::Shape::Rectangle(0, 0, 1, 1, sf::Color(255, 255, 255, 40) );
    myShape.SetBlendMode( sf::Blend::Add );
}

Manager::~Manager() {
    for( std::vector<GUI*>::iterator it = myGUIs.begin(); it != myGUIs.end(); ) {
        delete *it;
        myGUIs.erase(it);
    }
}

void Manager::Update( const sf::Vector2f mouse) {
    myMouse = mouse;

    for( std::vector<GUI*>::iterator it = myGUIs.begin(); it != myGUIs.end(); ++it)
        (*it)->Update();

    GUI * g;
    Widget * w;

    if ( (g =getGUI()) && (w = g->getWidget(myMouse)) ) {
        if ( w != myWidget ) {;
            myShape.SetPosition( w->getPosition() );
            myShape.SetPointPosition(1, w->getSize().x, 0);
            myShape.SetPointPosition(2, w->getSize().x, w->getSize().y);
            myShape.SetPointPosition(3, 0, w->getSize().y);
            myWidget = w;
        }
    }
    else myWidget = 0x00;
}

void Manager::Render( sf::RenderTarget & target) const {
    for( std::vector<GUI*>::const_iterator it = myGUIs.begin(); it != myGUIs.end(); ++it)
        target.Draw(**it);

    if ( myWidget ) target.Draw(myShape);
}

// SGUI::Manger::addGUI( SGUI::GUI * g)
void Manager::addGUI( GUI * g) {
    if ( !g ) return;
    myGUIs.push_back(g);
}

// SGUI::Manager::onGUI()
bool Manager::onGUI() {
    if ( getGUI() ) return true;
    return false;
}


// SGUI::Manager::getGUI()
GUI * Manager::getGUI() {
    for( std::vector<GUI*>::const_iterator it = myGUIs.begin(); it != myGUIs.end(); ++it) {
        GUI * g = *it;
        if ( myMouse.x >= g->myPos.x &&
             myMouse.x <= g->myPos.x + g->mySize.x &&
             myMouse.y >= g->myPos.y &&
             myMouse.y <= g->myPos.y + g->mySize.y ) return g;
    }
    return 0;
}


// SGUI::Manager::onButton()
bool Manager::onWidget() {
    GUI * g = getGUI();
    if ( !g ) return false;
    Widget * w = g->getWidget(myMouse);
    if ( !w ) return false;
    return true;
}

//SGUI::Manager::getButton()
Widget * Manager::getWidget() {
    if ( !getGUI()) return 0;
    return getGUI()->getWidget( myMouse);
}
