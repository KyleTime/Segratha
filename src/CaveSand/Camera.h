#ifndef CAMERAHEADER_H
#define CAMERAHEADER_H

#include <SFML/Graphics.hpp>

namespace Segratha
{
    /// @brief Handles operations related to camera position and size, use RenderWindow.SetView(...) with Camera::view to update your window
    class CAMERA
    {
        public:
            static constexpr float ASPECT_RATIO = 1.78; //aspect ratio (currently like 16:9 or some shit)
            static constexpr int SCREEN_SIZE = 800; //size of the actual window
            static constexpr int GAME_SIZE = 1200; //size of the game world (about 1200 works nicely)

            /// @brief The singleton View that the Camera should look at.
            static sf::View view;
            /// @brief A multiplier for how zoomed in the Camera should be
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