#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../CaveSand/CaveSand.h"

namespace Segratha
{
    class GameObject
    {
        public:
            const int STEPPY_RANGE = 3; //how large of a step can this object take? (const for now)

            bool checkGroundFrameTemp = false;

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

            #pragma region COLLISIONS

            /// @brief Checks all cells directly under the object's hitbox
            /// @param cave the CaveSand engine to query
            /// @return True if there is any ground under the object
            bool CheckGround(CaveSand* cave);

            /// @brief Checks all cells directly to the right or left of the object's hitbox
            /// @param cave the CaveSand engine to query
            /// @return True if either side collides with a cell
            bool CheckRightWall(CaveSand* cave);

            /// @brief Checks all cells directly to the right or left of the object's hitbox
            /// @param cave the CaveSand engine to query
            /// @return True if either side collides with a cell
            bool CheckLeftWall(CaveSand* cave);

            #pragma endregion
    };
}

#endif