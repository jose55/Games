#include <string>

#include "menu.hpp"

#include "game/database.hpp"

using namespace sf;

Menu::Menu( sf::RenderWindow * win) : Screen(win) {

    registerKey( Keyboard::G );
    registerKey( Keyboard::E );
    registerKey( Keyboard::S );

    myDb = Database::getDatabase();
    createText();
}


void Menu::Display() {
    Draw(&myTextMenu);
}

void Menu::Update() {
    if ( myKeys[Keyboard::G].pressed ) myNextScreen = Screen::ScreenGame;
    if ( myKeys[Keyboard::E].pressed ) myNextScreen = Screen::ScreenEditor;
    if ( myKeys[Keyboard::S].pressed ) {
        myDb->setSmooth( !myDb->getSmooth() );
        createText();
    }
}

void Menu::createText() {
    std::string myText = "========= Platform Evolved r007 =========";
    myText += "\n= Text Menu";
    myText += "\n= [ G ] Game Mode";
    myText += "\n= [ E ] Editor Mode";
    myText += "\n= [ S ] Toggle Image Smoothing (no smooth = retro)";
    myText += "\n==========================================";
    myText += "\n Smooth: ";
    if ( myDb->getSmooth() == 1 ) myText += "On";
    else myText += "Off";
    myTextMenu.SetString(myText);
}
