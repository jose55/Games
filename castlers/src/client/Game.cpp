#include "Game.hpp"

#include <cstdio>

using std::vector;

// tmp
int hitbox = 0;
int pause = 0;

bool space, fire, refire;
float ratio;
Clock btime;


// Ctor
Game::Game(sf::RenderWindow * win, Database * db) : Mode(win) {
    _db = db;

    _world = new World();
    _motions = new MotionSystem::MotionControl();

    /* CAMERA */
    _camera.SetFromRect( FloatRect(0, 0, 1024, 768));
    _win->SetView(_camera);
    _baseZoom = 1.0;
    _baseCameraX = _camera.GetCenter().x;
    _baseMoveX =0;


    // Debug
    Artillery * art = new Artillery(_db->GetArtillery("Catapult"));
    art->SetWindow(_win);
    art->SetPosition(Vector2f(20, 602));
    _units.push_back(art);

    _castle = new Castle(_db->GetCastleSprites());
    _drawers.push_back(_castle);
    _world->MergeObjects(_castle->GetBlocks());

    for(vector<Drawer*>::iterator it_Drawer = _drawers.begin(); it_Drawer != _drawers.end(); ++it_Drawer )
        (*it_Drawer)->SetWindow(_win);


    // Building Scene
    _scene.SetGround( _db->RegisterImage("res/scene/ground.png"));
    _scene.SetGrass( _db->RegisterImage("res/scene/grass.png"));
    _scene.SetSky( _db->RegisterImage("res/scene/sky.png"));
    _scene.AddElement( _db->RegisterImage("res/scene/dynamic/bg_hill01.png"), Scene::Background);
    _scene.AddElement( _db->RegisterImage("res/scene/dynamic/bg_hill02.png"), Scene::Background);
    _scene.AddElement( _db->RegisterImage("res/scene/dynamic/bg_hill03.png"), Scene::Background);
    _scene.AddElement( _db->RegisterImage("res/scene/dynamic/bg_hill04.png"), Scene::Background);
    _scene.AddElement( _db->RegisterImage("res/scene/dynamic/bg_hill05.png"), Scene::Background);
    _scene.AddElement( _db->RegisterImage("res/scene/dynamic/bg_hill06.png"), Scene::Background);
    _scene.Generate();

    // IN DEV
    _castle->Move(2248, 0);

    Sprite * hud_powerBG = new Sprite();
    Sprite * hud_powerArrow = new Sprite();
    Sprite * hud_controller = new Sprite();

    hud_controller->SetImage( *_db->RegisterImage("res/hud/unit_controller.png"));
    hud_controller->SetPosition( 40, _win->GetHeight() - hud_controller->GetSize().y - 20);


    hud_powerBG->SetImage( *_db->RegisterImage("res/hud/power_bg.png"));
    hud_powerBG->SetPosition( hud_controller->GetPosition() + Vector2f(74, - 16) );

    hud_powerArrow->SetImage(*_db->RegisterImage("res/hud/power_arrow.png"));
    hud_powerArrow->SetCenter(28, 9);
    hud_powerArrow->SetPosition( hud_powerBG->GetPosition() + Vector2f(38, 34) );
    hud_powerArrow->SetRotation(90);



    _HUD.AddElement(hud_powerBG, HUD_Manager::listBackground, true);
    _HUD.AddElement(hud_controller, HUD_Manager::listBackground, true);
    _idArrow = _HUD.AddElement(hud_powerArrow, HUD_Manager::listDynamic, true);


    fire = false;
    space = false;
    refire = true;
    // /DEV
}

Game::~Game() {
    _win->SetView( _win->GetDefaultView());
}



// Engine
bool Game::Update() {
        while(GetEvent(_event)) {
            // FABRICATION DE LA JAUGE
            if ( _event.Type == Event::KeyPressed && _event.Key.Code == Key::Space ) {
                if ( space == false && refire == true) {
                    space = true;
                    fire = false;
                    refire = false;
                    ratio = 0.0;
                    btime.Reset();
                }
            }
            if ( _event.Type == Event::MouseButtonPressed && _event.MouseButton.Button == Mouse::Left ) {
                Projectile * p = new Projectile();
                Artillery * a = static_cast<Artillery*>(_units[0]);
                a->LoadProjectile(p);
                _projectiles.push_back(p);
                _world->AddObject(p);
            }
            if ( _event.Type == Event::MouseButtonPressed && _event.MouseButton.Button == Mouse::Right ) {
                for( vector<Projectile*>::iterator it = _projectiles.begin(); it != _projectiles.end();) {
                    _world->DelObject((*it));
                    delete (*it);
                    _projectiles.erase(it);
                }
            }
            if ( _event.Type == Event::KeyPressed && _event.Key.Code == Key::H )
                hitbox ^= 1;

            if ( _event.Type == Event::KeyPressed && _event.Key.Code == Key::P )
                pause ^= 1;

            if ( _event.Type == Event::KeyPressed && _event.Key.Code == Key::N )
                if ( pause ) _world->Update();
        }
        /* INPUTS */
        if ( _win->GetInput().IsKeyDown(Key::Right) ) CameraMove(20);
        else if ( _win->GetInput().IsKeyDown(Key::Left)) CameraMove(-20);

        /* [Camera] Zoom Control */
        if ( _win->GetInput().IsKeyDown(Key::Down ) ) {
            if ( _baseZoom - 0.1 >= 0.3) {
                _baseZoom -= 0.1;
                float tmpmv = 382 *(-_baseZoom + 1);
                _camera.SetFromRect( FloatRect(_baseMoveX, 0-tmpmv, _baseMoveX+1024, 768-tmpmv  ));
                _camera.Zoom(_baseZoom);
            }
        }

        if ( _win->GetInput().IsKeyDown(Key::Up ) ) {
            if ( _baseZoom + 0.1 <= 1.0) {
                _baseZoom += 0.1;
                float tmpmv = 382 *(-_baseZoom + 1);
                _camera.SetFromRect( FloatRect(_baseMoveX, 0-tmpmv, _baseMoveX+1024, 768-tmpmv));
                _camera.Zoom(_baseZoom);
            }
        }


        if ( _win->GetInput().IsKeyDown(Key::Space ) ) {
            if ( space == true && fire == false ) {
                ratio = btime.GetElapsedTime() / 2.0;
                if ( ratio >= 1.0 ) {
                    ratio = 1.0;
                    fire = true;
                    space = false;
                }
                float angle = 90.0 - ( 180 * ratio);
                _HUD.GetElement(_idArrow)->SetRotation(angle);
            }

        }  else { if ( space == true ) { space = false; fire = true; } refire = true;}

        if ( fire ) {
                fire = false;
                _motions->AddMotion( _db->GetMotion("Catapult_Attack"), _units[0]);
        }

        /* PHYS ENGINE */
        if ( !pause ) _world->Update();

        /* ANIMATIONS */
        MotionSystem::MResp * resp = _motions->Compute();
        if ( resp ) {
            if ( resp->_value == MotionSystem::Fire )
                resp->_unit->Fire(ratio);
                Artillery * a = static_cast<Artillery*>(resp->_unit);
                a->CanonRotate(0);
        }

        return 1;
}

// Graphics
void Game::Display() {
    _win->SetView(_camera);
    _win->Draw(_scene);
    _motions->Apply();
    _castle->Draw();
    for( vector<Projectile*>::iterator it2 = _projectiles.begin(); it2 != _projectiles.end(); ++it2) {
        _win->Draw(*(*it2));
        if ( hitbox) _win->Draw(*(*it2)->DrawHitBox());
    }
    for( vector<Unit*>::iterator it = _units.begin(); it != _units.end(); ++it)
        (*it)->Draw();

    if ( hitbox) {
        _win->Draw(  *(_world->_cross.DrawHitBox())  );
        _win->Draw(  *(_world->_cross2.DrawHitBox()) );
    }

    _win->SetView(_win->GetDefaultView());
    _win->Draw(_HUD);
}

// Camera
void Game::CameraMove(float x) {
    if ( _camera.GetCenter().x + x >= _baseCameraX && _camera.GetCenter().x + x <= _baseCameraX + 2048 ) {
        _camera.Move(x, 0);
        _scene.Parallax(x);
        _baseMoveX += x;
    }
}
