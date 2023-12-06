#ifndef PLAYERCORE_H
#define PLAYERCORE_H

#include <SFML/Graphics.hpp>
#include "Game/PhysObject.h"

namespace Segratha
{
    class PlayerCore : public PhysObject
    {
        public:
            float speed = 40;
            float jump = -120;

            PlayerCore();
            ~PlayerCore();

            void Update(CaveSand* sand);
            void Draw(sf::RenderWindow* target);
    };
}

#endif