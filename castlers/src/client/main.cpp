#include <vector>
#include <cstdio>

#include <SFML/Graphics.hpp>

// Game Includes
#include "MainMenu.hpp"
#include "Game.hpp"
#include "Mode_Editor.hpp"

// Consts
#define GAME_W 1024
#define GAME_H 768
#define GAME_DEPTH 32

#define GAME_TITLE "Medevial"


using std::vector;
using namespace sf;

void Load(void* UserData);
bool loading;

int main(void) {
    RenderWindow * gameWindow = new RenderWindow(VideoMode(GAME_W, GAME_H, GAME_DEPTH), "Medieval" );
    Event gameEvents;
    Clock clockEngine;
    Database db;

    srand( time(NULL));

    Thread Loader(&Load, gameWindow);
    // Modes
    Mode * mode = new MainMenu(gameWindow);
    while(gameWindow->IsOpened() ) {
        mode->Mouse();

        while(gameWindow->GetEvent(gameEvents) ) {
            if ( gameEvents.Type == Event::Closed ||
                (gameEvents.Type == Event::KeyPressed && gameEvents.Key.Code == Key::Escape ) ) gameWindow->Close();
            else mode->AddEvent(gameEvents);
        }

        if ( clockEngine.GetElapsedTime() > 0.01667 ) { // 60 frames per second
            clockEngine.Reset();
            if ( !mode->Update() ) { // Mode Ended
                if ( mode->GetNext() == 0 ) break;
                int nextMode = mode->GetNext();
                delete mode;
                gameWindow->SetActive(false);
                loading = true;
                Loader.Launch();

                switch(nextMode) {
                    default:
                    case MODE_MMENU: { mode = new MainMenu(gameWindow); break; }
                    case MODE_GAME: { mode = new Game(gameWindow, &db); break;}
                    case MODE_EDITOR: { mode = new Editor(gameWindow, &db); break;}
                }
                loading = false;
                Loader.Wait();
                continue;
            }

        }
        gameWindow->Clear();
        mode->Display();
        gameWindow->Display();
    }
    delete mode;
    delete gameWindow;


    return EXIT_SUCCESS;

}

void Load(void* UserData) {
    Image i;
    Sprite s;
    i.LoadFromFile("res/screen_loading.png");
    s.SetImage(i);
    Clock t;
    t.Reset();
    RenderWindow * w = static_cast<RenderWindow*>(UserData);
    while( loading == true ) {
        w->Clear();
        w->Draw(s);
        w->Display();
    }
    w->SetActive(false);
}
