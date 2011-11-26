#include "Scene.hpp"

#include <cstdio>
#include <functional>

using namespace sf;
using namespace std;

Scene::Scene() { _width = 1024*3; }
Scene::Scene(float w) { _width = w; }

//
//  void Render(RenderTarget& target) const;
//  inherit: sf::Drawable
//  draw function
void Scene::Render(RenderTarget& target) const {
    unsigned int i;

    for(i = 0; i < _sky.size(); ++i)
        target.Draw(*_sky[i]);
    for(i = 0; i < _background.size(); ++i)
        target.Draw(*_background[i]);
    for(i = 0; i < _ground.size(); ++i)
        target.Draw(*_ground[i]);
    for(i = 0; i < _grass.size(); ++i)
        target.Draw(*_grass[i]);

}


void Scene::SetGround(sf::Image * img) {
    int nbimg = _width/img->GetWidth();
    nbimg += 2;

    // TODO: Clear old scene (if any?)
    for(int i = 0; i < nbimg; ++i) {
        Sprite * tmp = new Sprite( *img);
        tmp->SetX((i-1) * tmp->GetSize().x );
        tmp->SetY(600);
        _ground.push_back(tmp);
    }
}

void Scene::SetGrass(Image * img) {
    int nbimg = _width/img->GetWidth();
    nbimg += 2;

    // TODO: Clear old scene (if any?)
    for(int i = 0; i < nbimg; ++i) {
        Sprite * tmp = new Sprite( *img);
        tmp->SetX((i-1) * tmp->GetSize().x );
        tmp->SetY(600 - img->GetHeight());
        _grass.push_back(tmp);
    }
}

void Scene::SetSky(Image *img) {
    int nbimg = _width/img->GetWidth();
    nbimg += 2;

    // TODO: Clear old scene (if any?)
    for(int i = 0; i < nbimg; ++i) {
        Sprite * tmp = new Sprite( *img);
        tmp->SetX((i-1) * tmp->GetSize().x );
        tmp->SetCenter(0, tmp->GetSize().y );
        tmp->SetY(600);
        tmp->SetScaleY(2.0);
        _sky.push_back(tmp);
    }
}

void Scene::AddElement(Image * img, Category c, float ratio) {
    SceneElement * tmp = new SceneElement;
    tmp->_image = img;
    tmp->_cat = c;
    tmp->_ratio = ratio;
    _dynamic.push_back(tmp);
}

void Scene::Generate() {
    vector<SceneElement*>::iterator i;
    float ratio = 0.f, width = -1024.f;
    vector<SceneElement*>_backgroundSeed;

    // Creating Background;
    for(i = _dynamic.begin(); i != _dynamic.end(); ++i)
        if ( (*i)->_cat == Scene::Background ) { _backgroundSeed.push_back(*i); ratio += (*i)->_ratio; }

    while( width < _width ) {
            float r = (rand()%static_cast<int>(ratio*100));
            r /= 100.0;
            float finder = 0.0f;
            for(i = _backgroundSeed.begin(); i != _backgroundSeed.end(); ++i) {
                finder += (*i)->_ratio;
                if ( r < finder ) {
                    Sprite * tmp = new Sprite( *(*i)->_image );
                    tmp->SetX( width );
                    tmp->SetY( 600 - tmp->GetSize().y );
                    width += tmp->GetSize().x + ( rand()%200 - 100);
                    _background.push_back(tmp);
                    break;
                }
            }
    }
}

void Scene::Parallax(float x) {
    unsigned int i;
    for(i = 0; i < _sky.size(); ++i)
        _sky[i]->Move(8*x/10, 0);

    for(i = 0; i < _background.size(); ++i)
        _background[i]->Move(6*x/10, 0);
}

Scene::~Scene() {
    vector<Sprite*>::iterator i;
    vector<SceneElement*>::iterator i2;


    for(i = _ground.begin(); i != _ground.end(); ++i)
        delete *i;

    for(i = _grass.begin(); i != _grass.end(); ++i)
        delete *i;

    for(i = _background.begin(); i != _background.end(); ++i)
        delete *i;

    for(i = _sky.begin(); i != _sky.end(); ++i)
        delete *i;

    for(i2 = _dynamic.begin(); i2 != _dynamic.end(); ++i2)
        delete *i2;
}
