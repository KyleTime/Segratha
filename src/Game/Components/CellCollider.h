#ifndef CELLCOLLIDER_H
#define CELLCOLLIDER_H

#include "../Component.h"
#include "../../CaveSand/Cell.h"

namespace Segratha
{
    class CellCollider : public Component
    {
        public:
            const int STEPPY_RANGE = 3; //how large of a step can this object take? (const for now)

            sf::Vector2i cellScale; //amount of cells outward in either direction that the world hitbox extends. ie: if you have a cellScale of (1, 1), the hitbox is a 3x3 square

            cell_type collision_majority = AIR; //what element are we colliding with the most currently?

            //this struct contains an array that's the exact size needed to contain one of every cell_type
            //specifically for the CellCollide function to tell which materials the player is colliding with
            struct cellTypeList
            {
                int list[cell_type::Count];
                int size;
            };

            CellCollider(sf::Vector2i cellScale);

            void Awake() override;
            void Update() override;
            void Draw(sf::RenderWindow* target) override;

            /// @brief returns true if any part of the hitbox defined by cellScale is colliding a cell
            /// @param cave the CaveSand engine to query
            /// @return An array of every unique cell_type the player is colliding with
            cellTypeList CellCollide(CaveSand* cave);

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

            /// @brief Checks all cells directly above the object's hitbox
            /// @param cave the CaveSand engine to query
            /// @return True if there is ceiling
            bool CheckCeiling(CaveSand* cave);
    };
}

#endif