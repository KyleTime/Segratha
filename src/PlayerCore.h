#ifndef PLAYERCORE_H
#define PLAYERCORE_H

#include <SFML/Graphics.hpp>
#include "Game/PhysObject.h"

namespace Segratha
{
    class PlayerCore : PhysObject
    {
        public:
            PlayerCore();
            ~PlayerCore();

            void Update(CaveSand* sand);
            void Draw(sf::RenderWindow* target);
    };
}

#endif