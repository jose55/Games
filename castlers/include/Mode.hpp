#ifndef __CLASS_MODE__
#define __CLASS_MODE__

#include <cstdio>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Database.hpp"

#define MODE_MMENU 1
#define MODE_GAME 2
#define MODE_EDITOR 3

class Mode {
    public:
        Mode(sf::RenderWindow * win);
        virtual ~Mode();
        virtual bool Update() = 0;
        virtual void Display() = 0;

        void AddEvent(sf::Event e);
        bool GetEvent(sf::Event &e);

        // IN DEV
        void Mouse();

        int GetNext();
        void SetNext(int v);

    protected:
        sf::RenderWindow * _win;
        sf::Event _event;
        sf::Vector2f _mouse;
        Database * _db;

    private:
        std::vector<sf::Event>_events;
        int _next;
};

#endif
