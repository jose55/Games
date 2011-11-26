#ifndef __CLASS_SCENE__
#define __CLASS_SCENE__

#include <vector>

#include <SFML/Graphics.hpp>

class Scene : public sf::Drawable {
    public:
        Scene();
        Scene(float w);
        ~Scene();
        void Render( sf::RenderTarget& target) const;


        // v0.2
        enum Category { Background = 0, Sky = 1};
        void SetGround(sf::Image * img);
        void SetGrass(sf::Image * img);
        void SetSky(sf::Image * img);
        void AddElement(sf::Image * img, Category cat, float ratio = 1.0);
        void Generate();
        void Parallax(float x);


    private:
        struct SceneElement;
        float _width;
        std::vector<sf::Sprite*>_ground;
        std::vector<sf::Sprite*>_grass;
        std::vector<sf::Sprite*>_sky;
        std::vector<sf::Sprite*>_background;
        std::vector<SceneElement*>_dynamic;

        /* Dynamic Elements Struct */

    struct SceneElement {
        sf::Image * _image;
        Category _cat;
        float _ratio;
    };

};



#endif
