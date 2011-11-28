#include <iostream>

#include <game/animatedSprite.hpp>

using namespace std;


AnimatedSprite::AnimatedSprite() {
    myCurrentSequence = 0;
    myCurrentLength = 0;
}

AnimatedSprite::~AnimatedSprite() {
    for(int i = 0; i < animation_null; ++i)
        mySequences[i].clear();
}

void AnimatedSprite::setResource(sf::Texture * res) {
    mySprite.SetTexture(*res);
}


void AnimatedSprite::Render(sf::RenderTarget& target, sf::Renderer& renderer) const {
    target.Draw(mySprite);
}

void AnimatedSprite::Update() {
    if ( mySequences[myCurrentSequence].size() < 2 ) return;
    if ( myCurrentLength < (*myCurrentFrame).m_Length ) ++myCurrentLength;
    else {
        myCurrentLength = 0;
        if ( ++myCurrentFrame == mySequences[myCurrentSequence].end() ) myCurrentFrame = myLoopFrame;
        setCurrentFrame();
    }
}

void AnimatedSprite::FlipX(bool f) { mySprite.FlipX(f); }


void AnimatedSprite::addFrame( const sf::IntRect rect, int sequence, int length) {
    if ( sequence >= animation_null ) return;
    A_Frame f;
    f.m_Rect = rect;
    f.m_Length = length;
    mySequences[sequence].push_back(f);

    //
    if ( sequence == 0 && mySequences[0].size() == 1) {
        myCurrentFrame = mySequences[0].begin();
        myLoopFrame = myCurrentFrame;
        setCurrentFrame(0, 0);
    }
}

void AnimatedSprite::setCurrentFrame( int frame, int sequence ) {
    mySprite.SetSubRect( mySequences[sequence][frame].m_Rect );
    mySprite.SetOrigin( mySprite.GetSize().x/2, mySprite.GetSize().y);
}

void AnimatedSprite::setCurrentFrame() {
    mySprite.SetSubRect( (*myCurrentFrame).m_Rect );
    mySprite.SetOrigin( mySprite.GetSize().x/2, mySprite.GetSize().y);
}

void AnimatedSprite::setCurrentSequence( int sequence, int reset) {
    if ( myCurrentSequence == sequence && reset == 0 ) return;
    myCurrentSequence = sequence;
    myCurrentFrame = mySequences[myCurrentSequence].begin();
    myCurrentLength = 0;
    for( myLoopFrame = myCurrentFrame; myLoopFrame != mySequences[myCurrentSequence].end(); ++myLoopFrame ) {
        if ( (*myLoopFrame).m_Loop == 1 ) break;
    }
    if ( myLoopFrame == mySequences[myCurrentSequence].end() ) myLoopFrame = myCurrentFrame;

    setCurrentFrame();
}

const int AnimatedSprite::getCurrentSequence() { return myCurrentSequence; }

void AnimatedSprite::setSequenceLoopStart( int sequence, int frameStart) {
    mySequences[sequence][frameStart].m_Loop = 1;
}

const sf::Vector2f AnimatedSprite::GetSize() { return mySprite.GetSize(); }
