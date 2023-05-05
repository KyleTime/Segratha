#ifndef PLAYERCORE_H
#define PLAYERCORE_H

#include <SFML/Graphics.hpp>

namespace Segratha
{
    class PlayerCore
    {
        public:
            sf::Vector2f position;

            PlayerCore();
            ~PlayerCore();

            void Update();
            void Draw(sf::RenderWindow* target);
    };
}

#endif