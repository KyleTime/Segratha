#ifndef KINPUT_H
#define KINPUT_H

#include <SFML/Graphics.hpp>

namespace Segratha
{
    class KInput
    {
        public:
            KInput(sf::RenderWindow* window);

            static KInput* inst;

            sf::RenderWindow* window;

            //directional keys
            sf::Keyboard::Key right;
            sf::Keyboard::Key left;
            sf::Keyboard::Key up;
            sf::Keyboard::Key down;

            //yump
            sf::Keyboard::Key jump;

            //Mouse
            static sf::Vector2i GetMousePositionScreen();
            static sf::Vector2f GetMousePositionWorld();
            static sf::Vector2i GetMousePositionCell(); 

            ///@brief Calculates the direction in world coordinates from the given world position to the mouse
            ///@param fromWorld world position in question
            ///@return direction in radians
            static float DirectionToMouse(sf::Vector2f fromWorld);
    };
}

#endif