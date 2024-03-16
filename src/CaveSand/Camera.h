#ifndef CAMERAHEADER_H
#define CAMERAHEADER_H

#include <SFML/Graphics.hpp>

namespace Segratha
{
    /// @brief Handles operations related to camera position and size, use RenderWindow.SetView(...) with Camera::view to update your window
    class CAMERA
    {
        public:
            static constexpr float CAM_X = 640; //base size of window (width)
            static constexpr float CAM_Y = 360; //base size of window (height)
            static constexpr int SCREEN_SIZE = 2; //window multiplier (x1 [640x360] x2 [1280x720] x3 [1920x1080])

            /// @brief The singleton View that the Camera should look at.
            static sf::View view;
            /// @brief A multiplier for how zoomed in the Camera should be (on second thought don't mess with this too much)
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