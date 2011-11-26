#include <iostream>
#include <SFML/Graphics.hpp>

#include "screenManager.hpp"
#include "menu.hpp"
#include "game.hpp"
#include "editor.hpp"
#include "game/database.hpp"

using namespace sf;

int main(void) {
    RenderWindow win( VideoMode::GetDesktopMode() , "Platform - Evolved r007");

    Event event;
    Clock clock;
    Vector2f mouse;
    View view = win.GetDefaultView();

    const float beat = 1.f/60.f;


    Database::getDatabase()->Init();

    Screen * screen = new Menu(&win);
    //Screen * screen = new Editor(&win);
    //Screen * screen = new Game(&win);
    screen->setView(&view);
    screen->Init();
    clock.Reset();


    while( win.IsOpened() ) {
        while ( win.GetEvent(event) ) {
            if ( event.Type == Event::Closed ||
                (event.Type == Event::KeyPressed && event.Key.Code == Key::Escape) ) win.Close();
            else if ( event.Type == Event::KeyPressed && event.Key.Code != Key::Escape )
                screen->keyDown( event.Key.Code );
            if ( event.Type == Event::MouseButtonPressed && event.MouseButton.Button == Mouse::Left ) screen->updateMouseClick();
            if ( event.Type == Event::MouseButtonReleased && event.MouseButton.Button == Mouse::Left ) screen->releaseMouseClick();
        }
        mouse.x = win.GetInput().GetMouseX();
        mouse.y = win.GetInput().GetMouseY();
        screen->updateMousePos(mouse);

        if ( clock.GetElapsedTime() >= beat ) {
            clock.Reset();
            screen->updateScreen();
            int nextScreen;
            if ( (nextScreen = screen->nextScreen()) != 0) {
                delete screen;
                // Changing screen
                switch( nextScreen ) {
                    default: case Screen::ScreenMenu :
                    {
                        screen = new Menu(&win);
                        break;
                    }

                    case Screen::ScreenGame :
                    {
                        screen = new Game(&win);
                        break;
                    }

                    case Screen::ScreenEditor :
                    {
                        screen = new Editor(&win);
                        break;
                    }
                }
                view = win.GetDefaultView();
                screen->setView(&view);
                screen->Init();
                continue;

            }
        }

        win.Clear();
        win.SetView(view);
        screen->Display();
        win.SetView( win.GetDefaultView() );
        screen->DisplayHUD();
        win.Display();
    }


    return 0;
}
