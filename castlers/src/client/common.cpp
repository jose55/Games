#include "common.hpp"


void Snap(int x, int y, sf::Sprite * s) {
    s->SetPosition(x*32, y*32);
    int hblocks = s->GetSize().x/32;
    int vblocks = s->GetSize().y/32;
    if ( s->GetRotation() == 0.f ) {
        s->Move(s->GetSize().x/2, 0);
        s->Move(0, s->GetSize().y/2);
    }
    else {
        s->Move(s->GetSize().y/2, 0);
        if ( hblocks == vblocks )
            s->Move(0, s->GetSize().x/2);
    }
}
