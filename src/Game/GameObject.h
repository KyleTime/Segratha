#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "../CaveSand/CaveSand.h"
#include "Component.h"

namespace Segratha
{
    class GameObject
    {
        private:
            std::vector<Component*> components;
        public:
            sf::Vector2f position; //current position of the object
            sf::Vector2i cellPos; //current pixel cell of the object (updated every update cycle)

            GameObject();
            ~GameObject();
            
            //on the first frame the object is active, call this
            virtual void Awake();

            /// @brief A big function that combines all the necessary pieces to move the object by velocity and collide with things along the way
            /// @param cave CaveSand engine to query
            virtual void Update();

            //draws the object to the screen
            virtual void Draw(sf::RenderWindow* target);

            Component* AddComponent(Component* c);
            Component* GetComponent(std::string componentID);
    };
}

#endif