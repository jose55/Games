#include "screenManager.hpp"




Screen::Screen( sf::RenderWindow * win) : myWin(win) { myNextScreen = 0; }
Screen::~Screen() {}
void Screen::Draw( sf::Drawable * o) { myWin->Draw(*o); }

void Screen::updateMousePos( const sf::Vector2f pos) { myMouse = pos; }

void Screen::updateMouseClick() { myMouseLeft = 1; myMouseLeftDown = 1;}
void Screen::releaseMouseClick() { myMouseLeftDown = 0; }

void Screen::updateScreen() {
    Update();
    myMouseLeft = 0;
    // Updating key events
    for( std::map<int, key>::iterator it = myKeys.begin(); it != myKeys.end(); ++it) {
        if ( myWin->GetInput().IsKeyDown( (sf::Key::Code)it->first) )
            it->second.pressed = 0;
        else
            it->second.pressed = it->second.down = 0;
    }
}

void Screen::keyDown( int keyCode ) {
    std::map<int, key>::iterator it = myKeys.find(keyCode);
    if ( it != myKeys.end() ) {
        if ( it->second.down == 0 ) it->second.pressed = 1;
        it->second.down = 1;
    }
}


void Screen::registerKey( int keyCode ) {
    key p = { 0, 0 };
    myKeys.insert( std::make_pair(keyCode, p) );
}

int Screen::nextScreen() { return myNextScreen; }

void Screen::setView( sf::View * view) { myView = view; }
