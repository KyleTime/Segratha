#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "GameObject.h"

namespace Segratha
{
    class PhysObject : public GameObject
    {
        public:
            const float GRAVITY_SCALE = -100;

            sf::Vector2f velocity;
            bool grounded;
            bool doGravity;

            PhysObject();
            ~PhysObject();

            void Update(CaveSand* sand);
    };
}

#endif