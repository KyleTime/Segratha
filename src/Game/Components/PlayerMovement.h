#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "../Component.h"
#include "BasicPhysics.h"

namespace Segratha
{
    class PlayerMovement : public Component
    {
        private:
            BasicPhysics* phys;

            //HORIZONTAL MOVEMENT

            float maxSpeed = 10; //the speed that normal movement is capped at
            float acceleration = 1;
            float deceleration = 1;
            float airMod = 0.5f;

            //VERTICAL MOVEMENT

            float jumpPower;

            //INFORMATION
            
            bool grounded = false;

            /// @brief Using player input, make the character accelerate
            /// @param input the direction to accelerate in, should be -1 or 1
            void Accelerate(int input);

            /// @brief Slowly lower speed
            /// @param mul the modifier to the base deceleration rate (0, 1)
            void Decelerate(float mul);

        public:
            using Component::Component;

            void Awake() override;
            void Update() override;
            void Draw(sf::RenderWindow* target) override;

            void HandleRun();
            void HandleJump();
    };
}

#endif