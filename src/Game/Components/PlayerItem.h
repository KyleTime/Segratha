#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include "SpriteRenderer.h"

namespace Segratha
{
    //note: this component is intended to be used on an object separate from the player character
    class PlayerItem : public Component
    {
        private:
            SpriteRenderer* rend; //the item's renderer
            GameObject* player; //the gameobject of the player
        public:
            PlayerItem(SpriteRenderer* rend, GameObject* player);

            void Awake() override;
            void Update() override;
            void Draw(sf::RenderWindow* target) override;

            std::string GetComponentID() override;

            void MoveToPlayer(sf::Vector2f offset);
            void RotateToMouse(float radOffset);
    };
}

#endif