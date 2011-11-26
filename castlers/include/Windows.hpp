#ifndef __CLASS_WINDOWS__
#define __CLASS_WINDOWS__

#include <SFML/Graphics.hpp>

#include "Drawer.hpp"
#include "common.hpp"


#define ALIGN_H_CENTER (1<<0)
#define ALIGN_H_RIGHT (1<<1)
#define ALIGN_V_CENTER (1<<2)

#define ALIGN_CENTER 5

#define BUTTON_SINGLE 1
#define BUTTON_SWITCH 2

class TShape {
    public:
        TShape(sf::Vector2f size, const sf::Color &col);
        void SwitchColor(const sf::Color &col);
        void SetBaseColor(const sf::Color &col);
        void HighLight();
        virtual void Reset();

        // Getters
        inline sf::Vector2f GetPosition() { return _position; };
        inline sf::Vector2f GetSize() { return _size; };

        // Setters
        void SetSize(sf::Vector2f size);
        virtual void SetPosition(sf::Vector2f pos) = 0;
        void UseSpriteImage(sf::Sprite * spr, int p);
        void RotateImage(float angle);

    protected:
        sf::Shape _shape;
        sf::Vector2f _position, _size;
        sf::Color _sColor, _hlColor;
        sf::Sprite _sprite;
};

class TButton : public Drawer, public TShape {
    public:
        TButton(const char * text, int v, int btype);
        void Draw();
        bool Select();

        // Getters
        inline int GetValue() { return _value; };
        inline int GetType() { return _type; };
        // Setters
        void SetPosition(sf::Vector2f pos);

    private:
        sf::String _text;
        int _value;
        int _type;
        sf::Shape _select;
        int _selected;

};


class TWindow : public Drawer, public TShape {
    public:
        TWindow(sf::Vector2f size, sf::Vector2f pos);
        ~TWindow();
        void Draw();


        // Window related
        void AddButton(const char * text, int v, int btype = BUTTON_SINGLE, sf::Vector2f pos = sf::Vector2f(0, 0));
        TWindow * AddWindow(sf::Vector2f size, sf::Vector2f local);
        void Reset(); // Remove highlights
        void AlignSubWindow(TWindow * w, int p);

        // Getters
        TButton * GetButton(sf::Vector2f pos);

        // Setters
        void SetPosition(sf::Vector2f pos);

    private:
        std::vector<TButton*>_buttons;
        std::vector<TWindow*>_subwindows;
        // In Dev
        TButton * _cButton;
        TWindow * _cWindow;
};

#endif
