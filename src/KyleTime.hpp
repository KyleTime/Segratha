#ifndef KYLETIME_H
#define KYLETIME_H

#include <SFML/Graphics.hpp>

namespace Segratha
{
    class KyleTime
    {
        private:
            sf::Clock deltaClock;
            sf::Time deltaTime;
            static KyleTime* inst;

        public:
            static KyleTime* GetInstance()
            {
                if(inst == NULL)
                {
                    inst = new KyleTime;
                    return inst;
                }
                
                return inst;
            }

            //call this once at the start of the update loop each frame
            static void UpdateDelta()
            {
                inst->deltaTime = inst->deltaClock.restart();
            }

            //call this to return current delta time
            static float DeltaTime()
            {
                return inst->deltaTime.asSeconds();
            }
    };
}


#endif