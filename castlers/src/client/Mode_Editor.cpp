#include <cstdio>
#include <fstream>
#include <iostream>

#include "Mode_Editor.hpp"


using namespace sf;

using std::vector;
using std::ifstream;
using std::ios;
using namespace std;

Editor::Editor(RenderWindow * win, Database * db) : Mode(win) {
    _db = db;
    _gridBlock = true;

    _hLine = Shape::Line(0, 0, _win->GetWidth(), 0, 1, Color(100, 100, 100));
    _vLine = Shape::Line(0, 0, 0, _win->GetHeight(), 1, Color(100, 100, 100));
    _Ground.AddPoint(0, 600, sf::Color(66, 52, 4));
    _Ground.AddPoint(1024, 600, sf::Color(66, 52, 4));
    _Ground.AddPoint(1024, 768, sf::Color(50, 40, 0));
    _Ground.AddPoint(0, 768, sf::Color(50, 40, 0));

    /*************************************************************************************
    *   Windows Creation
    **************************************************************************************/
    // Préparation des fenêtres
    _toolWin = new TWindow(Vector2f(768, 32), Vector2f(16, 16));
    _wins.push_back(_toolWin);
    _dataWin = new TWindow(Vector2f(128, 192), Vector2f(_win->GetWidth() - 144, 48));
    _wins.push_back(_dataWin);
    for(vector<TWindow*>::iterator it = _wins.begin(); it != _wins.end(); ++it)
        (*it)->SetWindow(_win);
    _previewWin = _dataWin->AddWindow(Vector2f(64, 64), Vector2f(0, 16));
    _previewWin->SetBaseColor( Color(140, 140, 140) );

    _toolWin->AddButton("Castle", BRUSH_CASTLE, BUTTON_SWITCH);
    _toolWin->AddButton("Eraser", BRUSH_ERASE, BUTTON_SWITCH);
    _toolWin->AddButton("Save", ACTION_SAVE, BUTTON_SINGLE);
    _toolWin->AddButton("Load", ACTION_LOAD, BUTTON_SINGLE);

    _dataWin->AlignSubWindow(_previewWin, ALIGN_H_CENTER);
    _dataWin->AddButton("Rotate", ACTION_ROTATE, BUTTON_SINGLE, Vector2f(32, 84));
    _dataWin->AddButton("Flip H", ACTION_FLIPH, BUTTON_SWITCH, Vector2f(32,116));
    _dataWin->AddButton(">", ACTION_NEXT, BUTTON_SINGLE, Vector2f(100, 16));
    _dataWin->AddButton("<", ACTION_PREVIOUS, BUTTON_SINGLE, Vector2f(13, 16));

    _winSelect = 0x00;
    _buttonSelect = 0;

    _brush = 0;
    _brushAngle = 0;
    _brushFlipH = false;

    _castleSpr = _db->GetCastleSprites();

    _previewWin->UseSpriteImage(_castleSpr->spr, ALIGN_H_CENTER|ALIGN_V_CENTER);

}

bool Editor::Update() {
    Vector2i Cell = Vector2i( _mouse.x/32, _mouse.y/32);

    bool inWindow = false;

    /* Window Selector */
    for(vector<TWindow*>::iterator it = _wins.begin(); it != _wins.end(); ++it) {
        TWindow * tmp = (*it);
        if ( _mouse.x >= tmp->GetPosition().x && _mouse.x <= tmp->GetPosition().x + tmp->GetSize().x
            && _mouse.y >= tmp->GetPosition().y && _mouse.y <= tmp->GetPosition().y + tmp->GetSize().y ) {
                inWindow = true;
                if ( _winSelect) _winSelect->Reset();
                    _winSelect = tmp;
                    _winSelect->HighLight();
                break;
            }
    }
    if ( _winSelect ) {
        if ( !inWindow ) { // No Windows Selected
            _winSelect->Reset();
            _winSelect = 0x00;
        }
        else { // Let's look for any button
            TButton * b = _winSelect->GetButton(_mouse);
            if ( b ) _buttonSelect = b;
            else _buttonSelect = 0x00;
        }
    }
    else _buttonSelect = 0x00;


    while(GetEvent(_event)) {
        if ( _event.Type == Event::MouseButtonPressed && _event.MouseButton.Button == Mouse::Left ) {
            if ( _buttonSelect ) ButtonPressed(_buttonSelect);
            else if ( !_winSelect ) { //  Clicked on the grid
                switch(_brush) {
                    case BRUSH_CASTLE: brushCastle(); break;
                    case BRUSH_ERASE: brushErase(); break; // be back in a while
                }
            }
        }
    }
    return 1;
}


void Editor::Display() {
    Vector2i Cell = Vector2i( _mouse.x/32, _mouse.y/32);

    if ( _gridBlock ) {
        for(int i = 0; i < 32; ++i) {
            _hLine.SetY(i*32);
            _win->Draw(_hLine);
        }
        for(int i = 0; i < 32; ++i) {
            _vLine.SetX(i*32);
            _win->Draw(_vLine);
        }
    }
    // Ground
    _win->Draw(_Ground);

    // Helper
    _castleSpr->_shape.SetRotation(_brushAngle * 90);
    Vector2f rpos;
    if ( _brushAngle == 0 ) rpos = Vector2f(Cell.x*32 + _castleSpr->size.x/2, Cell.y*32 - _castleSpr->size.y/2 + 32 );
    else rpos = Vector2f(Cell.x*32 + _castleSpr->size.y/2, Cell.y*32 - _castleSpr->size.x/2 + 32 );

    _castleSpr->_shape.SetPosition(rpos);

    _win->Draw(_castleSpr->_shape);

    // Elements
    for(vector<Element*>::iterator it = _elements.begin(); it != _elements.end(); ++it) {
        _win->Draw(*((*it)->spr));
    }

    // Windows
    for(vector<TWindow*>::iterator it = _wins.begin(); it != _wins.end(); ++it)
        (*it)->Draw();
}

// BRUSH CASTLE
void Editor::brushCastle() {
    Vector2i Cell = Vector2i(_mouse.x/32, _mouse.y/32);
    Vector2f rpos;

    if ( _brushAngle == 0 ) rpos = Vector2f(Cell.x*32 + _castleSpr->size.x/2, Cell.y*32 - _castleSpr->size.y/2 + 32 );
    else rpos = Vector2f(Cell.x*32 + _castleSpr->size.y/2, Cell.y*32 - _castleSpr->size.x/2 + 32 );

    Element * e = new Element();
    e->spr = new Sprite( *_castleSpr->spr->GetImage());
    e->spr->SetCenter( Vector2f(_castleSpr->size.x/2, _castleSpr->size.y/2 ) );
    e->spr->SetRotation(_brushAngle*90);
    e->spr->SetPosition(rpos);
    if ( _brushFlipH ) {
        if ( _brushAngle ) e->spr->FlipY(true);
        else e->spr->FlipX(true);
    }

    BpBlock * block = new BpBlock();
    e->ptr = block;

    block->position = Cell;
    block->id = _castleSpr->_id;
    block->angle = _brushAngle;
    block->flip = ( _brushFlipH == 0 ) ? 0 : 1;

    _elements.push_back(e);
    _castle._blocks.push_back(block);
}

void Editor::brushErase() {
    for(vector<Element*>::iterator i = _elements.begin(); i != _elements.end(); ++i ) {
            Element * tmp = (*i);
            Vector2f esize, ecenter;
            if ( tmp->spr->GetRotation() == 0 ) {
                esize = Vector2f( tmp->spr->GetSize().x, tmp->spr->GetSize().y);
                ecenter = Vector2f( tmp->spr->GetCenter().x, tmp->spr->GetCenter().y);
            }
            else {
                esize = Vector2f( tmp->spr->GetSize().y, tmp->spr->GetSize().x);
                ecenter = Vector2f( tmp->spr->GetCenter().y, tmp->spr->GetCenter().x);
            }

            float x1 = tmp->spr->GetPosition().x - ecenter.x;
            float x2 = x1 + esize.x;
            float y1 = tmp->spr->GetPosition().y - ecenter.y;
            float y2 = y1 + esize.y;

            if ( _mouse.x < x1 ) continue;
            if ( _mouse.x > x2 ) continue;
            if ( _mouse.y < y1 ) continue;
            if ( _mouse.y > y2 ) continue;

            for(vector<BpBlock*>::iterator i2 = _castle._blocks.begin(); i2 != _castle._blocks.end(); ++i2) {
                BpBlock * b = (*i2);
                if ( tmp->ptr != b ) continue;
                printf("[Erase] Block at [%d][%d]\n", b->position.x, b->position.y);
                delete b;
                _castle._blocks.erase(i2);
                break;
            }
            delete tmp;
            _elements.erase(i);
            break;
    }
}


void Editor::ButtonPressed(TButton * b) {
    int v = _buttonSelect->GetValue();
    if ( _buttonSelect->Select() == true ) { // Activate a button
        switch(_buttonSelect->GetValue()) {
            case BRUSH_CASTLE:
            case BRUSH_ERASE:
            {
                _brush = v;
                break;
            }
            case ACTION_ROTATE:
            {
                if ( _brushAngle == 0 ) _brushAngle = 1;
                else _brushAngle = 0;
                _previewWin->RotateImage(_brushAngle*90);
                break;
            }
            case ACTION_SAVE:
            {
                printf("Saving Castle...\n");
                Save();
                break;
            }
            case ACTION_LOAD:
            {
                printf("Loading Castle...\n");
                Load();
                break;
            }
            case ACTION_NEXT:
            {
                _castleSpr = _castleSpr->next;
                _previewWin->UseSpriteImage(_castleSpr->spr, ALIGN_CENTER);
                break;
            }
            case ACTION_PREVIOUS:
            {
                _castleSpr = _castleSpr->previous;
                _previewWin->UseSpriteImage(_castleSpr->spr, ALIGN_CENTER);
                break;
            }
            case ACTION_FLIPH:
            {
                _brushFlipH = true;
                break;
            }

        }

    }
    else {
        if ( _brush == _buttonSelect->GetValue() ) _brush = 0;
        switch(v) {
            case ACTION_FLIPH:
            {
                _brushFlipH = false;
                break;
            }

        }
    }
}

void Editor::Save() {
    string castleName;
    cout << "[Save] Entrez le nom du chateau...: ";
    cin >> castleName;
    string path = "data/"+castleName+".dat";
    std::ofstream f (path.c_str(), ios::out | ios::binary);
    cout << "Saving to " << path << "..." << endl;

    int c = 0;
    for(vector<BpBlock*>::iterator i = _castle._blocks.begin(); i != _castle._blocks.end(); ++i) {
        ++c;
        BpBlock * b = (*i);
        f.write((char*) &(b->id), sizeof(int));
        f.write((char*) &(b->position.x), sizeof(int));
        f.write((char*) &(b->position.y), sizeof(int));
        f.write((char*) &(b->angle), sizeof(int));
        f.write((char*) &(b->flip), sizeof(int));
        printf("[Save] Block(%d) at [%d][%d] | Angle: %d, Flip: %d\n", b->id, b->position.x, b->position.y, b->angle, b->flip);
    }
    printf("[Save] %d Blocks saved\nDone!\n", c);
    f.close();
}

void Editor::Load() {
    string castleName;
    cout << "[Load] Entrez le nom du chateau...: ";
    cin >> castleName;
    string path = "data/"+castleName+".dat";
    ifstream f ( path.c_str(), ios::in | ios::binary);
    cout << "[Load] Opening " << path << "..." << endl;
    if ( !f.is_open() ) return;

    int c = 0;
    do {
        int id, x, y, a, flip;
        f.read( (char *) &id, sizeof(int));
        if ( f.eof() ) break;
        f.read( (char *) &x, sizeof(int));
        f.read( (char *) &y, sizeof(int));
        f.read( (char *) &a, sizeof(int));
        f.read( (char *) &flip, sizeof(int));
        Element * e = new Element();
        BpBlock * b = new BpBlock();
        Vector2f rpos;

        while(_castleSpr->_id != id ) _castleSpr = _castleSpr->next;
        if ( a == 0 ) rpos = Vector2f(x*32 + _castleSpr->size.x/2, y*32 - _castleSpr->size.y/2 + 32 );
        else rpos = Vector2f(x*32 + _castleSpr->size.y/2, y*32 - _castleSpr->size.x/2 + 32 );


        e->spr = new Sprite( *_castleSpr->spr->GetImage());
        e->spr->SetCenter( _castleSpr->spr->GetCenter());
        e->spr->SetRotation( a*90 );
        if ( flip == 1 ) {
            if ( a ) e->spr->FlipY(true);
            else e->spr->FlipX(true);
        }
        e->spr->SetPosition(rpos);
        e->ptr = b;

        b->id = id;
        b->angle = a;
        b->position = Vector2i(x, y);
        b->flip = flip;

        _elements.push_back(e);
        _castle._blocks.push_back(b);
        ++c;

        printf("[Load] Block(%d) at [%d][%d] | Angle: %d, Flip: %d\n", id, x, y, a, flip);

    } while ( !f.eof() );
    printf("[Load] %d Blocks loaded\nDone!\n", c);
}

Editor::~Editor() {
        delete _toolWin;
        delete _dataWin;
        delete _previewWin;
}
