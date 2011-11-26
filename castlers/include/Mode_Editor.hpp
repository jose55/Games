#ifndef __CLASS_MODE_EDITOR__
#define __CLASS_MODE_EDITOR__

#include <vector>

#include <SFML/Graphics.hpp>

#include "Mode.hpp"
#include "Windows.hpp"
#include "Blueprints.hpp"

#define BRUSH_CASTLE 1
#define BRUSH_ERASE 2

#define ACTION_FLIPV 6
#define ACTION_FLIPH 7
#define ACTION_ROTATE 8
#define ACTION_SAVE 9
#define ACTION_LOAD 10
#define ACTION_NEXT 11
#define ACTION_PREVIOUS 12

struct Element {
    Blueprint * ptr;
    sf::Sprite * spr;
};

class Editor : public Mode {
    public:
        Editor(sf::RenderWindow * win, Database * db);
        ~Editor();
        bool Update();
        void Display();

    private:
        void ButtonPressed(TButton * b);
        void Save();
        void Load();
        void brushCastle();
        void brushErase();

        bool _gridBlock;
        sf::Shape _hLine, _vLine, _Ground;

        TWindow * _toolWin, * _dataWin, * _previewWin, * _winSelect;
        TButton * _buttonSelect;

        // Brush
        int _brush;
        float _brushAngle;
        bool _brushFlipH;
        sf::Sprite _cursorSpr;

        BpCastle _castle;
        cSprite * _castleSpr;

        std::vector<Element*>_elements;
        std::vector<TWindow*>_wins;


};

#endif
