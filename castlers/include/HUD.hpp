#ifndef __NAMESPACE__HUD__
#define __NAMESPACE__HUD__

#include <vector>
#include <SFML/Graphics.hpp>

namespace HUD_Manager {
    enum HUD_List { listBackground = 0, listDynamic = 1};
    struct HUD_Element {
        sf::Sprite * _sprite;
        bool _destroy;
    };
    class HUD : public sf::Drawable {
        public:
            void Render(sf::RenderTarget &target) const;

            int AddElement(sf::Sprite * s, HUD_List l, bool autodelete = false);
            sf::Sprite * GetElement(int id);

            ~HUD();

        private:
            std::vector<HUD_Element*>_elementsBG;
            std::vector<HUD_Element*>_elementsFG;
            std::map<int, HUD_Element*>_index;
    };

};

#endif

