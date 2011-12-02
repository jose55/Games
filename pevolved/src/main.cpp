/*
*	main.cpp
*	Author: Salepate
*	version: 0.0
*/

#include <iostream>

#include <addons/engine/game.hpp>
#include <menu.hpp>

int main(int argc, char **argv) {
	Game::SetTitle("Platform Evolved - v0.0");
	Game::AddScreen(new Menu());
	Game::Start();
	std::cout << "Game Ended...\n";
	return 0;
}
