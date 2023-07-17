#ifndef PLAYERCORE_H
#define PLAYERCORE_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace Segratha
{
    class PlayerCore : GameObject
    {
        public:
            PlayerCore();
            ~PlayerCore();

            void Update(CaveSand* sand);
            void Draw(sf::RenderWindow* target);
    };
}

#endif