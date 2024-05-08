#ifndef PLAYERINVENTORY_H
#define PLAYERINVENTORY_H

#include "../Component.h"
#include "PlayerItem.h"

namespace Segratha
{
    class PlayerInventory : public Component
    {
        private:
            PlayerItem* items[4]; //pointers to all of our items, so that we can cycle through them

            int selected = 0; //index of active item

            void NextItem();
            void PreviousItem();
            void SelectItem(int index);

            void ShiftEnabled(); //goes through and disables all non-selected items and enables the selected one

        public:
            void Awake() override;

            void Update() override;
            void Draw(sf::RenderWindow* target) override;

            void AddItemToInventory(PlayerItem* item);
            void RemoveItemFromInventory(int index);
    };
}

#endif