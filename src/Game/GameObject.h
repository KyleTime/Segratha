#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../CaveSand/CaveSand.h"

namespace Segratha
{
    class GameObject
    {
        public:
            sf::Vector2f position; //current position of the object
            sf::Vector2i cellPos; //current pixel cell of the object (updated every update cycle)

            sf::Vector2i cellScale; //amount of cells outward in either direction that the world hitbox extends. ie: if you have a cellScale of (1, 1), the hitbox is a 3x3 square

            bool collideFrame = false; //have we collided with something this frame? (will prolly be removed later)

            GameObject();
            ~GameObject();

            /// @brief returns true if any part of the hitbox defined by cellScale is colliding a cell
            /// @param cave the CaveSand engine to query
            bool CellCollide(CaveSand* cave);

            /// @brief returns true if any part of the hitbox defined by cellScale is colliding a cell
            /// @param cave the CaveSand engine to query
            /// @param point an output to get the point of collision in Cell coordinates
            bool CellCollide(CaveSand* cave, sf::Vector2i& point, bool feet);

            /// @brief A big function that combines all the necessary pieces to move the object by velocity and collide with things along the way
            /// @param cave CaveSand engine to query
            virtual void Update(CaveSand* cave);
    };
}

#endif