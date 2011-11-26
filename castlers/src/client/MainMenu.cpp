#include "MainMenu.hpp"

using namespace sf;
using std::vector;

MainMenu::MainMenu(RenderWindow * win) : Mode(win) {
    _splashImage.LoadFromFile("res/mainmenu.bmp");
    _splash.SetImage(_splashImage);

    _wintmp = new TWindow(Vector2f(200, 32), Vector2f(10, _win->GetHeight()-42));
    _wintmp->SetWindow(_win);

    _wintmp->AddButton("Game", 1, BUTTON_SINGLE);
    _wintmp->AddButton("Editor", 2, BUTTON_SINGLE);
}

bool MainMenu::Update() {

    bool inWindow = false;
    bool leftButton = false;

    while(GetEvent(_event)) {
        if ( _event.Type == Event::MouseButtonPressed && _event.MouseButton.Button == Mouse::Left ) leftButton = true;
    }



    Vector2i Cell = Vector2i( _mouse.x/32, _mouse.y/32);
    TButton * b = 0x00;

    if ( _mouse.x >= _wintmp->GetPosition().x && _mouse.x <= _wintmp->GetPosition().x + _wintmp->GetSize().x
        && _mouse.y >= _wintmp->GetPosition().y && _mouse.y <= _wintmp->GetPosition().y + _wintmp->GetSize().y ) {
        inWindow = true;
        _wintmp->HighLight();
    }
    else _wintmp->Reset();

    if ( inWindow )
        b = _wintmp->GetButton(_mouse);

    if ( leftButton ) {
        if ( b ) {
            switch(b->GetValue()) {
                case 1: {
                    SetNext(MODE_GAME);
                    return 0;
                }
                case 2: {
                    SetNext(MODE_EDITOR);
                    return 0;
                }
            }
        }
    }
    return 1;
}

void MainMenu::Display() {
    _win->Draw(_splash);
    _wintmp->Draw();
}
