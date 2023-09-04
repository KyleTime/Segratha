#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "GameObject.h"

namespace Segratha
{
    class PhysObject : public GameObject
    {
        public:
            const float GRAVITY_SCALE = -120;

            sf::Vector2f velocity;
            bool grounded;
            bool doGravity;

            PhysObject();
            ~PhysObject();

            /// @brief Checks each direction based on velocity and applies proper collisions (based on functions in GameObject) 
            /// @param sand the CaveSand engine to query
            /// @return whether we collided with anything
            bool FullCollisionCheck(CaveSand* sand);

            void Update(CaveSand* sand);
    };
}

#endif