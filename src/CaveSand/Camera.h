#ifndef CAMERAHEADER_H
#define CAMERAHEADER_H

#include <SFML/Graphics.hpp>

namespace Segratha
{
    class CAMERA
    {
        public:
            static constexpr float ASPECT_RATIO = 1.78; //aspect ratio (currently like 16:9 or some shit)
            static constexpr int SCREEN_SIZE = 800; //size of the actual window
            static constexpr int GAME_SIZE = 1200; //size of the game world (about 1200 works nicely)

            static sf::View view;
            static float zoom;
            
            CAMERA();
            ~CAMERA();

            static void ChangeCenter(int x, int y);

            static void ChangeCenter(sf::Vector2f center);

            static void ChangeSize(int x, int y);

            static void ChangeSize(sf::Vector2f size);

            static float GetScreenHeight();
            static float GetScreenWidth();
    };
}

#endif