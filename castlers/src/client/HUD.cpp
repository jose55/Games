#include "HUD.hpp"
#include <cstdio>
using namespace HUD_Manager;
using namespace sf;
using std::vector;

// Drawer
void HUD::Render(RenderTarget &target) const {
    HUD_Element * e;
    unsigned int i;
    for(i = 0; i < _elementsBG.size(); ++i) {
        e = static_cast<HUD_Element*>(_elementsBG[i]);
        target.Draw(*e->_sprite);
    }

    for(i = 0; i < _elementsFG.size(); ++i) {
        e = static_cast<HUD_Element*>(_elementsFG[i]);
        target.Draw(*e->_sprite);
    }
}


int HUD::AddElement(sf::Sprite * s, HUD_List l,bool autodelete) {
    HUD_Element * tmp = new HUD_Element;
    tmp->_sprite = s;
    tmp->_destroy = autodelete;

    int id = _index.size();

    _index.insert( std::pair<int, HUD_Element*>(id, tmp));
    switch(l) {
        default: case listBackground: _elementsBG.push_back(tmp); break;
        case listDynamic: _elementsFG.push_back(tmp); break;
    }
    return id;
}

Sprite * HUD::GetElement(int id) {
    std::map<int, HUD_Element*>::iterator i = _index.find(id);
    if ( i != _index.end() ) return static_cast<HUD_Element*>(i->second)->_sprite;
    else return 0x00;
}


HUD::~HUD() {
    for(std::map<int, HUD_Element*>::iterator i = _index.begin(); i != _index.end();++i) {
        HUD_Element * tmp = (i->second);
        if ( tmp->_destroy ) delete tmp->_sprite;
        delete tmp;
    }

}
