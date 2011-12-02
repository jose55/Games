#include <iostream>
#include <string>

#include <menu.hpp>
#include <game/database.hpp>
#include <game.hpp>
#include <editor.hpp>

using namespace sf;

Menu::Menu() {

    myDb = Database::getDatabase();
    createText();
    Attach(myTextMenu);
}


int Menu::Update() {
	int exit = 0;
	if ( myKey[Keyboard::G].pressed ) {
		exit = 0;
		Game * scr = new Game();
		//AddScreen(scr);
		
    	}
	if ( myKey[Keyboard::E].pressed ) {
		exit = 1;
		Editor * scr = new Editor();
		//GetScreenList().push_back(scr);
	}
	if ( myKey[Keyboard::S].pressed ) {
		myDb->setSmooth( !myDb->getSmooth() );
		createText();
	    }
	if ( myKey[Keyboard::V].pressed ) exit = 1;
	return (update_alive && !exit);
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
