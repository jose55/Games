/*
 *	game.cpp
 *	author: Salepate
 *	version 0.0
 */

#include <iostream>
#include <vector>

#include <addons/engine/game.hpp>
#include <addons/engine/screen.hpp>

using namespace std;

void Game::Start() {
	if ( _init != 0 ) return;
	const unsigned int gameBeat = 1000/60; // ms 

	vector<Screen*>&gameScreens = Screen::GetScreenList();
	vector<Screen*>::iterator currentScreen;

	sf::Event gameEvent;
	sf::Clock gameClock;

	if ( !sf::Shader::IsAvailable() ) {
		sf::Err() << "warning, can't use Post FX...\n";
	}

	/*
	 * Initializing Game
	 */
	_window.Create( sf::VideoMode(800, 600, 32), _title);
	_init = 1;

	/*
	 *  Creating different Screens (credit, menu, game... etc)
	 *
	 */
	Screen::SetSize( _window );
	currentScreen = gameScreens.begin();

	gameClock.Reset();
	//(*currentScreen)->__init();
	// main game routine
	while ( _window.IsOpened() ) {
		Screen * screen = *currentScreen;
		/*
		*	Events Catcher
		*
		*/
		while( _window.PollEvent(gameEvent) )
			screen->__catchEvent(gameEvent);
		/*
		*	Game Updator
		*	60 times / second
		*	physic computing should go in there
		*/
		if ( gameClock.GetElapsedTime() > gameBeat ) {
			int state = screen->__update(_window);
			if ( state == Screen::update_die ) {
				if ( ++currentScreen == gameScreens.end() ) {
					_window.Close();
					continue;
				}
				//screen->__init();
			}
			gameClock.Reset();
		}

		// displaying
		_window.Clear();
		screen->__display(_window);
		_window.Display();
	}


	/* Game Destruction */
	_window.Close();
	for( 	currentScreen = gameScreens.begin();
		currentScreen != gameScreens.end();) {

		delete *currentScreen;
		gameScreens.erase(currentScreen);
	}
}

/* SetTitle
 */

void Game::SetTitle(const std::string title) {
	_title = title;
}
/* AddScreen
 *
 *
 *
 */
void Game::AddScreen(Screen* scr) {
	Screen::GetScreenList().push_back(scr);
}

/*
 *	Static members
 *	init is used to allow a single use of Game::Start()
 *	_window is the main window for our game
 *
 */
int Game::_init = 0;
std::string Game::_title = "My Game";
sf::RenderWindow Game::_window;
