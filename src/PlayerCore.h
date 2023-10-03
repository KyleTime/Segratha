#ifndef PLAYERCORE_H
#define PLAYERCORE_H

#include <SFML/Graphics.hpp>
#include "Game/PhysObject.h"

namespace Segratha
{
    class PlayerCore : public PhysObject
    {
        public:
            float speed = 20;
            float jump = -40;

            PlayerCore();
            ~PlayerCore();

            void Update(CaveSand* sand);
            void Draw(sf::RenderWindow* target);
    };
}

#endif