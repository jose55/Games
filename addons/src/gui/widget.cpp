#include <addons/gui/widget.hpp>

using namespace SGUI;
using namespace std;

//================================================================
// SGUI::Widget
//================================================================
Widget::Widget() {
    myPos = sf::Vector2f(0, 0);
    mySize = myPos;
    myValue = 0;
}

Widget::~Widget() {}
