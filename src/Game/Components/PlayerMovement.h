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

            float maxSpeed; //the speed that normal movement is capped at
            float acceleration;

            //VERTICAL MOVEMENT

            float jumpPower;

            void Accelerate(float multiplier);
            void Decelerate(float multiplier);

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