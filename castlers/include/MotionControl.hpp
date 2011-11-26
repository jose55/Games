#ifndef __MOTIONS__
#define __MOTIONS__

#include <map>
#include <vector>
#include <SFML/System.hpp>

#include "Artillery.hpp"

namespace MotionSystem {
    enum responses { Fire = 1};
    struct MResp {
        Unit * _unit;
        int _value;
    };

    struct MData {
        float _angle, _scale, _length;
        sf::Vector2f _dir;
    };

    class Motion {
        public:
            Motion();
            Motion(Motion& m);
            ~Motion();
            void AnimatePart(std::string part, float length, float angle, sf::Vector2f dir, float scale, int callback = 0, int callbacktime = 0 );
            MData * GetPart(std::string name);

            // need to be friended
            Unit * _unit;
            float _time, _length, _progress;
            int _callBack, _callBackTime;

        private:
            std::map<std::string, MData*>_parts;
    };

    class MotionControl {
        public:
            MotionControl();

            MResp * Compute();
            void Apply();

            void AddMotion(Motion * m, Unit * u);

        private:
            std::vector<Motion*>_motions;
            sf::Clock _clock;
            MResp _response;
    };

};

#endif
