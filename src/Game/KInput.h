#ifndef KINPUT_H
#define KINPUT_H

#include <SFML/Graphics.hpp>
#include "../KGenerics/LinkedList.h"
#include "GameObject.h"

namespace Segratha
{
    class KInput
    {
        public:
            KInput(sf::RenderWindow* window, LinkedList<GameObject*>* head);

            static KInput* inst;

            //note: I'm keeping a pointer of the list of gameobjects in the scene here so that we don't need to worry about it when other objects need it
            static LinkedList<GameObject*>* SceneObjects;

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