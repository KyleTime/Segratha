#ifndef HITPOINTS_H
#define HITPOINTS_H

#include "../Component.h"

namespace Segratha
{
    class HitPoints : public Component
    {
        private:
            float maxHitPoints;
            float hitPoints;
            bool destroyOnDeath = true;
        public:
            /// @brief HitPoints Constructor
            /// @param max Maximum Hit Points for this object
            /// @param destroyOnDeath Whether falling to 0 hit points removes this object
            HitPoints(float max, bool destroyOnDeath);

            void Awake() override;
            void Update() override;
            void Draw(sf::RenderWindow* target) override;

            std::string GetComponentID() override;

            float GetCurrentHitPoints();
            void Damage(float hp);
            void Heal(float hp);
            void Set(float hp);

            void Die();
    };
}

#endif