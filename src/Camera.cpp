#include "Camera.h"

using namespace Segratha;

float CAMERA::zoom = 1.f;
sf::View CAMERA::view = sf::View();

CAMERA::CAMERA()
{

}

CAMERA::~CAMERA()
{
}

void CAMERA::ChangeCenter(int x, int y)
{
    view.setCenter(x, y);
}

void CAMERA::ChangeCenter(sf::Vector2f center)
{
    view.setCenter(center);
}

void CAMERA::ChangeSize(int x, int y)
{
    view.setSize(x, y);
}

void CAMERA::ChangeSize(sf::Vector2f size)
{
    view.setSize(size);
}
