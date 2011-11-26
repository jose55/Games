/*
*   Drawer Class
*
*   - version 0.1
*   Optimize all classes that need to draw onto screen
*/
#include "Drawer.hpp"


using namespace std;
using namespace sf;

/*  SetWindow
*   @params:
*       - win.. obvious..hu
*/
void Drawer::SetWindow(RenderWindow * win) { _win = win; }

Sprite * Drawer::GetSprite(string name) {
    map<string, Sprite*>::iterator it = _sprites.find(name);
    if ( it != _sprites.end() ) return it->second;
    return 0x00;
}


void Drawer::AddSprite(string name, Sprite * s) {
    map<string, Sprite*>::iterator it = _sprites.find(name);

    if ( it == _sprites.end() ) {
        Sprite * ns = new Sprite(*s->GetImage());
        ns->SetCenter( s->GetCenter());
        _sprites.insert(pair<string, Sprite*>(name, ns));
    }
}

Drawer::~Drawer() {
    for(map<string, Sprite*>::iterator i = _sprites.begin(); i != _sprites.end();) {
        delete (i->second);
        _sprites.erase(i++);
    }

}

void Drawer::DrawBack() {}
