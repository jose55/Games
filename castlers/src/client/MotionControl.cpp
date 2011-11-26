#include "MotionControl.hpp"

using namespace MotionSystem;

using namespace std;


MotionControl::MotionControl() { _clock.Reset(); }
MResp * MotionControl::Compute() {
    bool resp = false;
    float t = _clock.GetElapsedTime();

    for( vector<Motion*>::iterator i = _motions.begin(); i != _motions.end();) {
        Motion * m = *i;
        float progress = t - m->_time;
        if ( progress < m->_length ) {
            m->_progress =  progress/m->_length;
            ++i;

        }
        else {
            if ( m->_callBackTime == -1 ) {
                resp = true;
                _response._value = m->_callBack;
                if ( m->_callBack == Fire )
                    _response._unit = m->_unit;
            }
            delete m;
            _motions.erase(i);
            continue;
        }
    }
    if ( !resp )
        return 0x00;
    else return &_response;
}

void MotionControl::Apply() {
    for(vector<Motion*>::iterator i = _motions.begin(); i != _motions.end(); ++i) {
        Motion * m = *i;
        float progress = m->_progress;
        Artillery * u = static_cast<Artillery*>(m->_unit);
        u->CanonRotate(progress * m->GetPart("canon")->_angle );
    }
}

void MotionControl::AddMotion(Motion * m, Unit * u) {
    Motion * nm = new Motion(*m);
    nm->_unit = u;
    nm->_time = _clock.GetElapsedTime();
    _motions.push_back(nm);

}

/* MOTIONS */
Motion::Motion() {

}

Motion::Motion(Motion& m) {
    _length = 0;
    for(map<string, MData*>::iterator i = m._parts.begin(); i != m._parts.end(); ++i) {
        MData * d = new MData;
        d->_angle = i->second->_angle;
        d->_scale = i->second->_scale;
        d->_dir = i->second->_dir;
        d->_length = i->second->_length;
        if ( d->_length > _length ) _length = d->_length;
        string n = i->first;
        _parts.insert(pair<string, MData*>( n, d));
    }

    _callBack = m._callBack;
    _callBackTime = m._callBackTime;
}

void Motion::AnimatePart(std::string part, float length, float angle, sf::Vector2f dir, float scale, int callback, int callbacktime) {
    MData * m = new MData;
    m->_angle = angle;
    m->_scale = scale;
    m->_dir = dir;
    m->_length = length;

    _parts.insert( std::pair<std::string, MData*>(part, m));

    if ( callback )
        _callBack = callback;

    if ( callbacktime )
        _callBackTime = callbacktime;
}

MData * Motion::GetPart(string name) {
    map<string, MData*>::iterator i = _parts.find(name);
    if ( i != _parts.end() ) return i->second;
    return 0x00;
}

Motion::~Motion() {
    for(std::map<std::string, MData*>::iterator i = _parts.begin(); i != _parts.end();) {
        delete (i->second);
        _parts.erase(i++);
    }
}
