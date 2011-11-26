/*
*   Database Class
*
*   - version 0.0
*/
#include "Database.hpp"
/*
*
*/
using std::map;
using std::pair;
using std::string;
using namespace sf;

Database::Database() {
    /* Catapult */
    Drawer * tmpc = RegisterItem(ITEM_TYPE_UNIT, UNIT_TYPE_ARTILLERY, "Catapult");
    Artillery * art = static_cast<Artillery*>(tmpc);
    Sprite * sprtmp = new Sprite();
    sprtmp->SetImage(*RegisterImage("res/catapult.bmp"));
    art->SetBody(sprtmp);
    sprtmp = new Sprite();
    sprtmp->SetImage(*RegisterImage("res/catapult_launcher.bmp"));
    art->SetCanon(sprtmp);
    art->SetCanonCenter(Vector2f(69, 6));
    art->SetCanonPosition(Vector2f(85, -66));

    // Catapult.Motion::Fire
    MotionSystem::Motion * attack = new MotionSystem::Motion();
    attack->AnimatePart("canon", 0.3, 80.0, Vector2f(0, 0), 0, 1, -1);

    _dbMotions.insert(pair<string, MotionSystem::Motion*>("Catapult_Attack", attack));

    /*************************************************************************************
    *   Castle Edition
    ***************************************************************************************/
    _castleSpr = new cSprite();
    _castleSpr->spr = new Sprite(*RegisterImage("res/castle_block_2x1.bmp"));
    _castleSpr->spr->SetCenter( 32, 16);
    _castleSpr->pos = Vector2f(0, 0);
    _castleSpr->size = _castleSpr->spr->GetSize();
    _castleSpr->_shape.AddPoint(0, 0, Color::Blue);
    _castleSpr->_shape.AddPoint(64, 0, Color::Blue);
    _castleSpr->_shape.AddPoint(64, 32, Color::Blue);
    _castleSpr->_shape.AddPoint(0, 32, Color::Blue);
    _castleSpr->_shape.SetCenter(32, 16);
    _castleSpr->_id = 1;
    _castleSpr->next = new cSprite();

    cSprite * tmp = _castleSpr->next;
    tmp->spr = new Sprite(*RegisterImage("res/castle_block_1x1.bmp"));
    tmp->spr->SetCenter( tmp->spr->GetSize().x/2, tmp->spr->GetSize().y/2 );
    tmp->pos = Vector2f(0, 0);
    tmp->size = tmp->spr->GetSize();
    tmp->_shape.AddPoint(0, 0, Color::Blue);
    tmp->_shape.AddPoint(32, 0, Color::Blue);
    tmp->_shape.AddPoint(32, 32, Color::Blue);
    tmp->_shape.AddPoint(0, 32, Color::Blue);
    tmp->_shape.SetCenter(16, 16);
    tmp->_id = 2;
    _castleSpr->previous = tmp;
    tmp->previous = _castleSpr;
    tmp->next = new cSprite();

    tmp = tmp->next;
    tmp->spr = new Sprite(*RegisterImage("res/castle_block_2x2_1.bmp"));
    tmp->spr->SetCenter( tmp->spr->GetSize().x/2, tmp->spr->GetSize().y/2 );
    tmp->pos = Vector2f(0, 0);
    tmp->size = tmp->spr->GetSize();
    tmp->_shape.AddPoint(0, 0, Color::Blue);
    tmp->_shape.AddPoint(64, 0, Color::Blue);
    tmp->_shape.AddPoint(64, 32, Color::Blue);
    tmp->_shape.AddPoint(32, 32, Color::Blue);
    tmp->_shape.AddPoint(32, 64, Color::Blue);
    tmp->_shape.AddPoint(0, 64, Color::Blue);
    tmp->_shape.SetCenter(32, 32);
    tmp->_id = 3;
    tmp->next = _castleSpr;
    tmp->previous = _castleSpr->next;
    _castleSpr->previous = tmp;
}

Database::~Database() {
    for(map<string, Artillery*>::iterator it = _dbArtillery.begin(); it != _dbArtillery.end(); ++it)
        delete it->second;
    for(map<string, Image*>::iterator it2 = _dbImage.begin(); it2 != _dbImage.end(); ++it2)
        delete it2->second;
    for(map<string, MotionSystem::Motion*>::iterator it3 = _dbMotions.begin(); it3 != _dbMotions.end(); ++it3)
        delete it3->second;
}

Image * Database::RegisterImage(string path) {
    map<string, Image*>::iterator it = _dbImage.find(path);
    if ( it != _dbImage.end() ) // Image exists
        return it->second;
    Image * tmp = new Image();
    if ( !tmp->LoadFromFile(path) ) // Image couldn't be loaded
        return 0x00;
    tmp->CreateMaskFromColor(Color(255, 0, 255));
    tmp->SetSmooth(false);
    _dbImage.insert(pair<string, Image*>(path, tmp));
    return tmp;
}

Drawer * Database::RegisterItem(int itype, int etype, const char name[]) {
    switch(itype) {
        default:
        return 0x00;
        break;

        case ITEM_TYPE_UNIT:
        {
            Artillery * tmp = new Artillery();
            _dbArtillery.insert( pair<string, Artillery*>( name, tmp));
            return tmp;
        }
    }
}

Artillery * Database::GetArtillery(const char name[]) {
    map<string, Artillery*>::iterator it;
    it = _dbArtillery.find(name);
    if ( it != _dbArtillery.end() ) return it->second;
    return 0x00;
}

cSprite * Database::GetCastleSprites() { return _castleSpr; }

MotionSystem::Motion * Database::GetMotion(std::string name) {
    map<string, MotionSystem::Motion*>::iterator it = _dbMotions.find(name);
    if ( it != _dbMotions.end() ) // Motion exists
        return it->second;
    return 0x00;
}
