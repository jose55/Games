#ifndef __screenEditor__
#define __screenEditor__

#include <addons/engine/screen.hpp>

namespace SGUI { class Manager; class GUI; class Alert; };
class Database;
class Stage;
class Platform;
class Enemy;

class Editor : public Screen {
    public:
        Editor();
        ~Editor();
        int Update();
	inline void GetEvent(sf::Event&) {}

    private:
        SGUI::Manager * myGUI;
        SGUI::GUI * myToolBar;
        SGUI::Alert * myAlert;

        Database * myDb;
        Enemy * myEnemy;

        // Brusher
        enum brushes { brush_platform = 0, brush_enemy, brush_spawn, brush_null};
        enum actions { action_save = brush_null+1, action_load };
        int myBrush;
        Platform * myPlatform;

        Stage * myStage;

        // Helper
        sf::Shape myPlatformHelper;

        sf::Text myStatus;
        sf::Clock myStatusClock;
        float myStatusLength;

        // Buffers
        sf::Vector2i myCell;

	/* tmp */
	int myMouseLeft, myMouseLeftDown;
};

#endif
