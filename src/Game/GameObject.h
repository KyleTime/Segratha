#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "../CaveSand/CaveSand.h"
#include "Component.h"
#include "../KGenerics/LinkedList.h"

namespace Segratha
{
    class GameObject
    {
        protected:
            std::vector<Component*> components;
            bool garbage = false; //if this is set to true, the main function should garbage collect this object
        public:
            sf::Vector2f position; //current position of the object
            sf::Vector2i cellPos; //current pixel cell of the object (updated every update cycle)

            GameObject();
            ~GameObject();
            
            //on the first frame the object is active, call this
            void Awake();

            /// @brief A big function that combines all the necessary pieces to move the object by velocity and collide with things along the way
            /// @param cave CaveSand engine to query
            void Update();

            //draws the object to the screen
            void Draw(sf::RenderWindow* target);

            //the object's behaviour on death
            virtual void Die();
            //returns whether this GameObject has been destroyed and needs to be cleaned up
            bool Garbage();

            Component* AddComponent(Component* c);
            Component* GetComponent(std::string componentID);
    };
}

#endif