#include <cstdio>

#include "Windows.hpp"

using namespace sf;

using std::vector;


/********************************************************************************************
*   TShape Class
*
*   - version 0.0
********************************************************************************************/
TShape::TShape(Vector2f size, const Color &col = Color(255, 255, 255, 255) ) {
    _position = Vector2f(0, 0);
    _size = size;
    _sColor = col;
    _hlColor = Color(col.r+40, col.g+40, col.b+40);
    _shape = Shape::Rectangle(0, 0, _size.x, _size.y, Color::White, 1, Color(col.r-30, col.g-30, col.b-30));
    _shape.SetColor(col);
}

void TShape::SetSize(sf::Vector2f size) {
    _size = size;
    _shape = Shape::Rectangle(0, 0, _size.x, _size.y, Color::White, 1, Color(_sColor.r-30, _sColor.g-30, _sColor.b-30));
    _shape.SetColor(_sColor);
}

// Change Shape appearance
void TShape::SwitchColor(const Color &col) { _shape.SetColor(col); }

void TShape::SetBaseColor(const sf::Color &col) {
    _sColor = col;
    _hlColor = Color(col.r+40, col.g+40, col.b+40);
    SwitchColor(col);
}

void TShape::HighLight() { _shape.SetColor(_hlColor); }

void TShape::Reset() { _shape.SetColor(_sColor); }

void TShape::UseSpriteImage(sf::Sprite * spr, int p) {
    Vector2f subsize = Vector2f(spr->GetImage()->GetWidth(), spr->GetImage()->GetHeight() );
    _sprite.SetSubRect( IntRect(0, 0, subsize.x, subsize.y));
    _sprite.SetImage(*spr->GetImage());
    _sprite.SetCenter( subsize.x/2, subsize.y/2);

    Vector2f subpos = Vector2f( subsize.x/2, subsize.y/2);

    if ( (p&ALIGN_H_CENTER) )
        subpos.x = GetSize().x/2;
    if ( (p&ALIGN_V_CENTER) )
        subpos.y = GetSize().y/2;

    _sprite.SetPosition(GetPosition().x+subpos.x, GetPosition().y+subpos.y);
}

void TShape::RotateImage(float angle) { _sprite.SetRotation(angle); }
/********************************************************************************************
*   TButton Class
*
*   - version 0.1
********************************************************************************************/
TButton::TButton(const char * text, int v, int btype) : TShape( Vector2f(0, 0), Color(140, 140, 140, 255) ) {
    _text.SetText(text);
    _text.SetColor(Color::Black);
    _text.SetSize(18);
    _text.SetFont(Font::GetDefaultFont());
    _value = v;
    _selected = 0;
    _type = btype;

    SetSize(Vector2f(_text.GetRect().Right + 4, 28));
    SetPosition( Vector2f(0, 0));
}

void TButton::Draw() {
    _win->Draw(_shape);
    _win->Draw(_text);
    if ( _selected > 0 )
        _win->Draw(_select);
    if ( _type == BUTTON_SINGLE && _selected > 0 ) _selected--;
}

bool TButton::Select() {
    if ( _selected > 0 ) _selected = 0;
    else _selected = 60;
    return _selected;
}

void TButton::SetPosition(Vector2f pos) {
    _position = pos;
    _shape.SetPosition(pos);
    _text.SetPosition(Vector2f(pos.x+2, pos.y+2));
    _select = Shape::Rectangle(pos.x, pos.y, pos.x+GetSize().x, pos.y+GetSize().y, Color(100, 100, 200) );
    _select.SetBlendMode( Blend::Multiply );
}

/********************************************************************************************
*   TWindow Class
*
*   - version 0.0
********************************************************************************************/
// Constructor
TWindow::TWindow(Vector2f size, Vector2f pos) : TShape(size, Color(120, 120, 160, 255)) {
    _position = pos;
    _shape.SetPosition(pos);
    _cButton = 0x00;
    _cWindow = 0x00;
}

// Draw
void TWindow::Draw() {
    _win->Draw(_shape);
    _win->Draw(_sprite);
    for(vector<TButton*>::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
        (*it)->Draw();
    for(vector<TWindow*>::iterator it2 = _subwindows.begin(); it2 != _subwindows.end(); ++it2)
        (*it2)->Draw();
}


/* Window related functions */

// Add a button to the window
void TWindow::AddButton(const char * text, int v, int btype, Vector2f pos) {
    TButton * t = new TButton(text, v, btype);
    t->SetWindow(_win);

    if ( pos.x == 0 && pos.y == 0 ) {
        pos.x = 2;
        for(vector<TButton*>::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
            pos.x += (*it)->GetSize().x+4;
        pos.y = 2;
    }
    t->SetPosition(GetPosition()+pos);

    _buttons.push_back(t);
}

// Add a sub window
TWindow * TWindow::AddWindow(Vector2f size, Vector2f local) {
    TWindow * tmp = new TWindow(size, Vector2f(GetPosition().x + local.x, GetPosition().y + local.y) );
    tmp->SetWindow(_win);
    _subwindows.push_back(tmp);
    return tmp;
}

// Remove any highlights onto the window
void TWindow::Reset() {
    SwitchColor(_sColor);
    if ( _cButton ) _cButton->Reset();
    if ( _cWindow ) _cWindow->Reset();
}

void TWindow::AlignSubWindow(TWindow * w, int p) {
    if ( !w ) return;

    Vector2f subpos = w->GetPosition();
    Vector2f subsize = w->GetSize();

    Vector2f center = Vector2f(GetPosition().x + GetSize().x/2, GetPosition().y + GetSize().y/2 );

    if ( (p&ALIGN_H_CENTER) ) {
        w->SetPosition( Vector2f( center.x - subsize.x/2, subpos.y) );
    }

}

// Return int attached to any button located at pos
TButton * TWindow::GetButton(Vector2f pos) {
    bool inWindow = false;
    for(vector<TWindow*>::iterator itw = _subwindows.begin(); itw != _subwindows.end(); ++itw) {
        TWindow * tmpw = (*itw);
        if ( pos.x >= tmpw->GetPosition().x && pos.x <= tmpw->GetPosition().x + tmpw->GetSize().x
            && pos.y >= tmpw->GetPosition().y && pos.y <= tmpw->GetPosition().y + tmpw->GetSize().y ) {
                inWindow = true;
                if ( _cWindow) _cWindow->Reset();
                    _cWindow = tmpw;
                    _cWindow->HighLight();
                break;
            }
    }
    if ( _cWindow && !inWindow ) {
        _cWindow->Reset();
        _cWindow = 0x00;
    }

    for(vector<TButton*>::iterator it = _buttons.begin(); it != _buttons.end(); ++it) {
        TButton * tmp = (*it);
        if ( pos.x >= tmp->GetPosition().x && pos.x <= tmp->GetPosition().x + tmp->GetSize().x
            && pos.y >= tmp->GetPosition().y && pos.y <= tmp->GetPosition().y + tmp->GetSize().y ) {
                if ( _cButton ) _cButton->Reset();
                _cButton = tmp;
                _cButton->HighLight();
                return tmp;
            }
    }
    if ( _cButton ) _cButton->Reset();

    return 0x00;
}

//
void TWindow::SetPosition(sf::Vector2f pos) {
    _shape.SetPosition(pos);
    // Move all buttons correctly
    for(vector<TButton*>::iterator it = _buttons.begin(); it != _buttons.end(); ++it) {
        TButton * tmp = (*it);
        Vector2f newpos;
        newpos.x = tmp->GetPosition().x - _position.x + pos.x;
        newpos.y = tmp->GetPosition().y - _position.y + pos.y;
        tmp->SetPosition(newpos);
    }
    _position = pos;
}

// no memory leak, wooooooord.
TWindow::~TWindow() {
    for(vector<TButton*>::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
        delete (*it);
    for(vector<TWindow*>::iterator itw = _subwindows.begin(); itw != _subwindows.end(); ++itw)
        delete (*itw);
}
