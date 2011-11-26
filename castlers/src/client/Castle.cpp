/*
*   Castle Class
*
*   - version 0.0
*/
#include <fstream>

#include "Castle.hpp"

using std::ifstream;
using std::ios;


extern int hitbox;
/*
*
*/
Block::Block() : Physic( Physic::hbBB ) {}

void Block::physicMoved() { _spr->SetPosition(GetPosition()); }

// Ctor
Castle::Castle(cSprite * blocklist) {
    _castleSpr = blocklist;
    Load();
    _win = 0x00;
}

// Drawer
void Castle::Draw() {
    for(vector<Physic*>::iterator it = _blocks.begin(); it != _blocks.end(); ++it) {
        Block * tmp = static_cast<Block*>(*it);
        if ( (*it)->Hit(0));
        else {
            _win->Draw(*(tmp->_spr));
            if ( hitbox) _win->Draw(*tmp->DrawHitBox());
        }
    }
}

vector<Physic*> * Castle::GetBlocks() { return &_blocks; }
void Castle::AddBlock(Block * b) { _blocks.push_back(b); }

void Castle::Move(float x, float y) {
    for(vector<Physic*>::iterator i = _blocks.begin(); i != _blocks.end(); ++i ) {
        Block * b = static_cast<Block*>(*i);
        b->Move(x, y);
    }
}


void Castle::Load() {
    ifstream f ("data/castle.dat", ios::in | ios::binary);
    if ( !f.is_open() ) {
        printf("Could not load castle\n");
        return;
    }

    _blocks.clear();

    while ( !f.eof() ) {
        int id, x, y, angle, flip;
        f.read((char*)&id, sizeof(int));
        if ( f.eof() ) break;
        f.read((char*)&x, sizeof(int));
        f.read((char*)&y, sizeof(int));
        f.read((char*)&angle, sizeof(int));
        f.read((char*)&flip, sizeof(int));

        while(_castleSpr->_id != id) _castleSpr = _castleSpr->next;

        Block * tmp = new Block();
        tmp->_spr = new Sprite( *_castleSpr->spr->GetImage());
        tmp->_spr->SetCenter( _castleSpr->spr->GetImage()->GetWidth()/2, _castleSpr->spr->GetImage()->GetHeight()/2);
        tmp->_spr->SetRotation(angle*90);
        if ( !angle )
            tmp->_spr->SetPosition(x*32 + tmp->_spr->GetCenter().x, y*32 - tmp->_spr->GetCenter().y);
        else tmp->_spr->SetPosition(x*32 + tmp->_spr->GetCenter().y, y*32 - tmp->_spr->GetCenter().x);
        if ( flip ) {
            if ( angle ) tmp->_spr->FlipY(true);
            else tmp->_spr->FlipX(true);
        }
        if ( angle == 0 )
            tmp->SetSize(_castleSpr->size);
        else tmp->SetSize( _castleSpr->size.y, _castleSpr->size.x );
        tmp->Snap(x, y);
        tmp->SetBreakable(true);
        _blocks.push_back(tmp);
        printf("[Load] Block(%d) at [%d][%d], Angle: %d / %d\n", id, x, y, angle, flip);
    }
    f.close();

}
