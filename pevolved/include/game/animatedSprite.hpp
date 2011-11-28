#ifndef __animSprite__
#define __animSprite__

#include <SFML/Graphics.hpp>

enum Animations { animation_idle = 0, animation_walk, animation_jump, animation_attack, animation_custom1, animation_null };

struct A_Frame {
    sf::IntRect m_Rect;
    int m_Length, m_Loop;
};

class AnimatedSprite : public sf::Drawable {
    public:
        AnimatedSprite();
        virtual ~AnimatedSprite();
        void setResource( sf::Texture * res );

        void Render( sf::RenderTarget&, sf::Renderer&) const;
        void Update();

        //void SetPosition( const sf::Vector2f pos);
        void FlipX(bool f);
        const sf::Vector2f GetSize();

        void addFrame( const sf::IntRect rect, int sequence, int length=1);
        void setCurrentFrame( int frame, int sequence );
        void setCurrentFrame();
        void setCurrentSequence( int sequence, int reset = 0);
        const int getCurrentSequence();
        void setSequenceLoopStart( int sequence, int frameStart);

    private:
        sf::Sprite mySprite;

        std::vector<A_Frame>mySequences[12];

        int myCurrentSequence, myCurrentLength;
        std::vector<A_Frame>::iterator myCurrentFrame, myLoopFrame;

};

#endif
