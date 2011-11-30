#ifndef __classGame__
#define __classGame__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Screen;

class Game {
	public:
		static void AddScreen(Screen*);
		static void Start();
		static void SetTitle(const std::string);
	private:
		static int _init;
		static sf::RenderWindow _window;
		static std::string _title;
};


#endif
