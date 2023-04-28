#ifndef PLAYERCORE_H
#define PLAYERCORE_H

#include <SFML/Graphics.hpp>

class PlayerCore
{
    public:
        sf::Vector2f position;

        PlayerCore();
        ~PlayerCore();
};

#endif