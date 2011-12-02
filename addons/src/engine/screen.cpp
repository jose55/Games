//== File Information =========================================================
//= File: 	screen.cpp
//= Author:	Salepate
//= Version:	0.03
//= Updated:	30/11/11 11:23
//=
//== File Description =========================================================
//= Screen Handler (help manipulate screens) - source code
//=
//== License ==================================================================
//= This program is free software: you can redistribute it and/or modify
//= it under the terms of the GNU General Public License as published by
//= the Free Software Foundation, either version 3 of the License, or
//= (at your option) any later version.
//=
//= This program is distributed in the hope that it will be useful,
//= but WITHOUT ANY WARRANTY; without even the implied warranty of
//= MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//= GNU General Public License for more details.
//=
//= You should have received a copy of the GNU General Public License
//= along with this program.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================

#include <iostream>

// Screen Module
#include <addons/engine/screen.hpp>

using namespace std;

Screen::Screen() {
	for(unsigned int i = 0; i < layer_count; ++i) {
		Layer * l = new Layer();
		pair<layers,Layer*>pair(layers(i), l);
		_layers.insert(pair);
	}
}

Screen::~Screen() {
	for( map<layers, Layer*>::iterator it = _layers.begin();
			it != _layers.end(); ++it) {
		delete it->second;
		_layers.erase(it);
	}
}

void Screen::SetSize( sf::RenderWindow &win) {
	_size.x = win.GetWidth();
	_size.y = win.GetHeight();
}

vector<Screen*>& Screen::GetScreenList() { return _screenList; }

void Screen::__init() {
	_clock.Reset();
	Init();
}

/* <IN-DEV:Salepate> */
// Fuckin' messy
int Screen::__update( sf::RenderWindow &win) {
	unsigned int cTime = GetTime();
	for( map<sf::Keyboard::Key, Key>::iterator it = myKey.begin();
			it != myKey.end(); ++it) {
		Key& k = it->second;
		if ( k.event > 0 ) {
			k.pressed = k.down ^ 1;
			k.down = 1;
			k.event = 2;
		}
	}
	for ( vector<SFX*>::iterator it = _sfx.begin();
			it != _sfx.end();) {
		SFX * sfx = *it;
		if ( cTime >= sfx->end ) { // look for decays
			delete *it;
			_sfx.erase(it);
			continue;
		}
		if ( cTime < sfx->start ) { // ignore unborn babies
			++it;
			continue;
		}
		int instant = cTime - sfx->start;
		float ratio = (float)instant / (float)(sfx->end - sfx->start);
		// Send ratio to Shader
		//sfx->shader.SetParameter("ratio", ratio);
		++it;

	}
	return Update();
}

void Screen::__display( sf::RenderWindow &win)  {
	for( std::map<layers, Layer*>::iterator it = _layers.begin();
			it != _layers.end(); ++it) { // For each layer
		Layer * l = it->second;
		std::vector< sf::Drawable*>&v = l->entities;
		std::vector<sf::Drawable*>::iterator it2;
		for( it2 = v.begin(); it2!= v.end(); ++it2) {
			if ( l->sfx )
				win.Draw(*(*it2), l->sfx->shader);
			else
				win.Draw(*(*it2));
		}
	}
}

void Screen::__catchEvent( sf::Event& e) {
	if ( e.Type == sf::Event::KeyPressed ) {
		Key& k = myKey[e.Key.Code];
		if ( k.event == 0 )
			k.event = 1;
	}

	else if ( e.Type == sf::Event::KeyReleased ) {
		Key& k = myKey[e.Key.Code];
		k.event = 0;
		k.pressed = k.down = 0;
	}
	else
		GetEvent(e);
}

void Screen::GetEvent(sf::Event&) {}
void Screen::Init() {}

void Screen::Trigger(layers layer, int sfx, unsigned int delay, unsigned int length) {
	SFX *tmp = new SFX();
	tmp->start = GetTime()+delay;
	tmp->end = GetTime()+delay+length;
	/*cout << "Start Time: " << tmp->start
	     << "End Time: " << tmp->end << "\n";*/
	tmp->shader.LoadFromFile("res/shader/sample01.sfx");
	_sfx.push_back(tmp);
	_layers[layer]->sfx = tmp;
}

void Screen::Attach( sf::Drawable &obj, layers layer) {
	_layers[layer]->entities.push_back(&obj);
}

/* </IN-DEV> */


unsigned int Screen::GetTime() { return _clock.GetElapsedTime(); }
const sf::Vector2i& Screen::GetSize() { return _size; }

sf::Clock 	Screen::_clock;
sf::Vector2i 	Screen::_size(0, 0);
vector<Screen*>	Screen::_screenList;
