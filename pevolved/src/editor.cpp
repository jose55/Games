#include <iostream>


#include "editor.hpp"
#include "addons/guiManager.hpp"
#include "game/stage.hpp"
#include "game/platform.hpp"
#include "game/enemy.hpp"
#include "game/database.hpp"

using namespace SGUI;
using namespace std;

Editor::Editor( sf::RenderWindow * win) : Screen(win) {
    myDb = Database::getDatabase();

    myGUI = new Manager();

    myToolBar = new GUI();
    myToolBar->setSize(36*8, 32);
    myToolBar->setShape( sf::Color(200, 100, 100) );

    // Brush Platform
    Button * b1, *b2;
    IntBox * ib1, *ib2;
    Label * lb1;

    b1= new Button( brush_platform );
    b1->setImage( myDb->getImage("res/gui/editor/iconBrushPlatform.png") );
    myToolBar->addWidget(b1, sf::Vector2f(2, 2));
    b2 = b1;

    b1 = new Button( brush_enemy );
    b1->setImage( myDb->getImage("res/gui/editor/iconBrushEnemy.png"));
    myToolBar->addWidget(b1, b2->getPosition() + sf::Vector2f(4 + b2->getSize().x, 0) );
    b2 = b1;

    b1 = new Button( brush_spawn );
    b1->setImage( myDb->getImage("res/gui/editor/iconSpawnPoint.png"));
    myToolBar->addWidget(b1, b2->getPosition() + sf::Vector2f(4 + b2->getSize().x, 0) );
    b2 = b1;

    b1 = new Button( action_save );
    b1->setImage( myDb->getImage("res/gui/editor/iconActionSave.png") );
    myToolBar->addWidget(b1, b2->getPosition() + sf::Vector2f(4 + b2->getSize().x, 0) );
    b2 = b1;

    b1 = new Button( action_load );
    b1->setImage( myDb->getImage("res/gui/editor/iconActionLoad.png") );
    myToolBar->addWidget(b1, b2->getPosition() + sf::Vector2f(4+b2->getSize().x, 0));
    b2 = b1;

    myGUI->addGUI(myToolBar);

    GUI * winArea = new GUI();

    winArea->setImage( myDb->getImage("res/gui/editor/bgWinArea.png"));
    winArea->setPosition( myToolBar->getPosition() + sf::Vector2f( myToolBar->getSize().x, 0));

    //winArea->setProperty( property_draggable );

    lb1 = new Label();
    lb1->setText("Size X:");
    winArea->addWidget(lb1, sf::Vector2f(4, 4));

    ib1 = new IntBox();
    ib1->setValue(800);
    winArea->addWidget(ib1, sf::Vector2f(4, lb1->getPosition().y + lb1->getSize().y+4));
    ib2 = ib1;

    lb1 = new Label();
    lb1->setText("Size Y:");
    winArea->addWidget(lb1, sf::Vector2f(4, ib1->getPosition().y + ib1->getSize().y + 4));

    myGUI->addGUI(winArea);


    myAlert = new Alert();
    myAlert->setPosition( myToolBar->getPosition() + sf::Vector2f(myToolBar->getSize().x, 0 ));
    myGUI->addGUI(myAlert);


    // Init
    myBrush = Editor::brush_null;

    myStage = new Stage();

    myStatus.SetPosition(36*8+4, 0);

    myPlatform = 0x00;
    myPlatformHelper = sf::Shape::Rectangle(0, 0, 32, 32, sf::Color::Blue);
    myPlatformHelper.SetBlendMode( sf::Blend::Add );

    myEnemy = myDb->getEnemy("basic");

    registerKey( sf::Key::Back );
}

Editor::~Editor() {
    delete myGUI;
    delete myStage;

    myAlert = 0x00;
    myToolBar = 0x00;
    myDb = 0x00;

}

void Editor::Display() {
    Area * area = myStage->getArea(0);
    Draw(area);
    for( vector<Enemy*>::const_iterator it = area->myEnemies.begin(); it != area->myEnemies.end();  ++it)
        Draw(*it);
    if ( myBrush != brush_null ) Draw(&myPlatformHelper);
    Draw(myGUI);

    if ( myStatusClock.GetElapsedTime() < myStatusLength ) Draw(&myStatus);
}

void Editor::Update() {
    if ( myKeys[sf::Key::Back].pressed ) {
        myStage->getArea(0)->Clear();
        myNextScreen = ScreenMenu;
    }
    sf::Vector2i mouseCell((int)myMouse.x/32,(int)myMouse.y/32);

    myGUI->Update( myMouse );


    /*
    *   Mouse is on GUI
    */
    if ( myGUI->onGUI() ) {

        /*
        *   TOOLBAR
        */
        if ( myGUI->getGUI( ) == myToolBar ) {
            if ( myMouseLeft && myGUI->onWidget() && myGUI->getWidget()->getType() == widget_button ) {
                Button * btn = static_cast<Button*>(myGUI->getWidget());

                if ( myBrush == btn->getValue() ) myBrush = brush_null;

                else switch(btn->getValue() ) {
                    case brush_platform:
                    case brush_enemy:
                    case brush_spawn: {
                        myBrush = btn->getValue();
                        break;
                    }

                    case action_save: {
                        myStage->Save();
                        myAlert->setText("Map saved...");
                        myAlert->Display();
                        break;
                    }

                    case action_load: {
                        myStage->Load();
                        myAlert->setText("Map loaded...");
                        myAlert->Display();
                        break;
                    }
                }
            }
        }


    }
    /*
    *   Otherwise
    */
    else {
        if ( myBrush != brush_null ) myPlatformHelper.SetPosition( mouseCell.x*32, mouseCell.y*32);
        // =========================================================================================
        //  Brush: Platform
        // =========================================================================================
        if ( myBrush == brush_platform ) {
            if ( !myPlatform) {
                if ( myMouseLeft ) {
                    int erase = 0;
                    Area * a = myStage->getArea(0);

                    for( vector<Platform*>::iterator it = a->myPlatforms.begin(); it != a->myPlatforms.end(); ++it) {
                        Platform * p = *it;
                        if ( p->getPos().x > myMouse.x || p->getPos().x + p->getSize().x < myMouse.x ||
                             p->getPos().y > myMouse.y || p->getPos().y + p->getSize().y < myMouse.y ) continue;
                        delete p;
                        a->myPlatforms.erase(it);
                        erase = 1;
                        break;
                    }

                    if ( !erase ) {

                        Platform * p = new Platform();
                        myPlatform = p;
                        p->setPos( sf::Vector2f(mouseCell.x*32, mouseCell.y*32) );
                        myCell = mouseCell;
                        a->myPlatforms.push_back(p);
                    }
                }
            }
            else {
                if ( myMouseLeftDown ) {
                    int x_dif = mouseCell.x - myCell.x;
                    int y_dif = mouseCell.y - myCell.y;
                    if ( x_dif < 0 ) {
                        myPlatform->setX( myCell.x*32+x_dif*32 );
                        x_dif = -x_dif;
                    }
                    else myPlatform->setX( myCell.x*32);

                    if ( y_dif < 0 ) {
                        myPlatform->setY( myCell.y*32+y_dif*32 );
                        y_dif = -y_dif;
                    }
                    else myPlatform->setY( myCell.y*32);
                    ++x_dif;
                    ++y_dif;
                    myPlatform->setSize( sf::Vector2f(x_dif*32,y_dif*32));
                }
                else {
                    myPlatform->materialize();
                    myPlatform = 0;
                }
            }
        }
        // =========================================================================================
        //  Brush: Enemy
        // =========================================================================================
        if ( myBrush == brush_enemy ) {
            if ( myMouseLeft ) {
                int erase = 0;
                Area * area = myStage->getArea(0);

                for( vector<Enemy*>::iterator it = area->myEnemies.begin(); it != area->myEnemies.end(); ++it) {
                    sf::FloatRect bb = (*it)->getBB();
                    if ( myMouse.x < bb.Left || myMouse.x > bb.Right
                        || myMouse.y < bb.Top || myMouse.y > bb.Bottom ) continue;

                    delete (*it);
                    area->myEnemies.erase(it);
                    erase = 1;
                    break;
                }

                if ( !erase ) {
                    Enemy * e1 = new Enemy(*myEnemy);
                    e1->setPos( sf::Vector2f(mouseCell.x*32 + e1->getSize().x/2, mouseCell.y*32+32) );
                    area->myEnemies.push_back(e1);
                }
            }
        }


    }
}
