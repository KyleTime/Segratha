#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "GameObject.h"


namespace Segratha
{
    enum phys_state : unsigned char { OPEN_AIR, SWIMMING };

    class PhysObject : public GameObject
    {
        public:
            const float GRAVITY_SCALE = -120;
            const float TERMINAL_VELOCITY = 250;

            sf::Vector2f velocity;
            //used in a switch statement to determine what kind of movement the object is doing
            phys_state state;
            bool grounded;

            PhysObject();
            ~PhysObject();

            /// @brief Checks each direction based on velocity and applies proper collisions (based on functions in GameObject) 
            /// @param sand the CaveSand engine to query
            /// @return whether we collided with anything
            bool FullCollisionCheck(CaveSand* sand);

            //figure out how gravity changes this frame
            void GravityUpdate();

            //add some drag to the movement based on the element the player is in contact with
            void Drag();

            //figures out if we are wet or not basically
            void StateUpdate(CaveSand* sand);

            //uses the current velocity to move the object
            //segmented into 4 steps for the integrity of collision detection
            void MoveSegmented(CaveSand* sand);

            void Update(CaveSand* sand);
    };
}

#endif