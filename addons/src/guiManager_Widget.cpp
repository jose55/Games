#include <addons/guiManager.hpp>

using namespace SGUI;
using namespace std;

//================================================================
// SGUI::Widget
//================================================================
Widget::Widget() {
    myPos = sf::Vector2f(0, 0);
    mySize = myPos;
    myValue = 0;
}

Widget::~Widget() {}

//================================================================
// SGUI::Label
//================================================================
Label::Label() {
    myType = widget_label;
    myText.SetSize( 16.f);
}

void Label::setPosition( const sf::Vector2f & p) {
    myPos = p;
    myText.SetPosition(p);
}

void Label::Render( sf::RenderTarget & target) const {
    target.Draw(myText);
}

void Label::setText( std::string text) {
    myText.SetText(text);
    mySize.x = myText.GetRect().Right;
    mySize.y = myText.GetRect().Bottom;
}

//================================================================
// SGUI::Button
//================================================================
Button::Button( int return_value ) {
    myShape = 0x00;
    mySprite = 0x00;
    setValue( return_value );
    myType = widget_button;
}

Button::~Button() {
    if ( myShape ) delete myShape;
    //cout << "Button deleted successfuly";
}

// SGUI::Button::setPosition( float x, float y)
void Button::setPosition( float x, float y) {
    myPos = sf::Vector2f(x, y);
    if ( myShape ) myShape->SetPosition(myPos);
    if ( mySprite ) mySprite->SetPosition(myPos);
}
void Button::setPosition( const sf::Vector2f & p) { setPosition( p.x, p.y); }


// SGUI:Button::setImage( sf::Image * image)
void Button::setImage( sf::Image * image) {
    if ( !mySprite ) mySprite = new sf::Sprite();
    mySprite->SetImage( *image );
    mySprite->SetPosition(myPos);
    mySize = mySprite->GetSize();

}


// SGUI::Render( sf::RenderTarget & target) const
void Button::Render( sf::RenderTarget & target) const {
    if ( myShape ) target.Draw(*myShape);
    if ( mySprite ) target.Draw(*mySprite);
}

//================================================================
// SGUI::IntBox
//================================================================

IntBox::IntBox() {
    myBox = sf::Shape::Rectangle(0, 0, 48, 24, sf::Color(0xC0, 0xC0, 0xC0), 1, sf::Color::White);
    myValue = 800;
    myText.SetText( "0" );
    myText.SetSize( 16.f);
    myType = widget_box;
    mySize.x = 48;
    mySize.y = 24;
}

void IntBox::Render( sf::RenderTarget & target) const {
    target.Draw(myBox);
    target.Draw(myText);
}

void IntBox::setPosition( const sf::Vector2f & p) {
    myPos = p;
    myBox.SetPosition(p);
    myText.SetPosition(p + sf::Vector2f(4, 0));
}

void IntBox::setValue( int val) {

}
