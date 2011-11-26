#include "Mode.hpp"

Mode::Mode(RenderWindow * win) { _win = win;  _next = 0; }
Mode::~Mode() {}

void Mode::AddEvent(sf::Event e) {
    _events.push_back(e);
}

bool Mode::GetEvent(sf::Event &e) {
    if ( _events.size() > 0 ) {
        e = _events.back();
        _events.pop_back();
        return true;
    }
    return false;
}


void Mode::Mouse() { _mouse = _win->ConvertCoords(_win->GetInput().GetMouseX(),_win->GetInput().GetMouseY());  }


void Mode::SetNext(int v) { _next = v; }
int Mode::GetNext() { return _next; }
