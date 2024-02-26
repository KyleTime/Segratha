#ifndef COMPONENT_H
#define COMPONENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../CaveSand/KyleTime.hpp"

namespace Segratha
{
    class GameObject;

    class Component
    {
        protected:
            GameObject* gameObject;
        public:
            Component();
            ~Component();

            virtual void Awake();

            virtual void Update();
            virtual void Draw(sf::RenderWindow* target);

            virtual std::string GetComponentID();

            void SetParent(GameObject* parent);
            GameObject* GetParent();
    };
} // namespace Segratha


#endif