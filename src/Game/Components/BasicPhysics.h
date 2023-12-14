#ifndef BASICPHYSICS_H
#define BASICPHYSICS_H

#include "../Component.h"

namespace Segratha
{
    class BasicPhysics: Component
    {
        private:

        public:
            using Component::Component;

            void Awake() override;
            void Update() override;
            void Draw(sf::RenderWindow* target) override;
    };
}

#endif