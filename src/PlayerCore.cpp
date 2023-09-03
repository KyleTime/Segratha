#include "PlayerCore.h"
#include "CaveSand/KyleTime.hpp"
#include "CaveSand/Camera.h"

using namespace Segratha;

PlayerCore::PlayerCore()
    : PhysObject()
{
    cellScale = sf::Vector2i(3, 3);
}

PlayerCore::~PlayerCore() {}

void PlayerCore::Update(CaveSand* sand)
{
    float mod = 1;

    velocity.x = 0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        mod = 5;
    else
        mod = 1;
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        velocity.y = jump * KyleTime::DeltaTime();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x = speed * mod * KyleTime::DeltaTime();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x = -speed * mod * KyleTime::DeltaTime();
    }

    PhysObject::Update(sand);
    CAMERA::ChangeCenter(position);
}

void PlayerCore::Draw(sf::RenderWindow* target)
{
    static CaveSand* inst = CaveSand::GetInstance();

    sf::RectangleShape shape(sf::Vector2f(20, 20));
    shape.setPosition(position.x - 10, position.y - 10);

    float xSize = cellScale.x * CELL_SIZE * 2;
    float ySize = cellScale.y * CELL_SIZE * 2;
    sf::RectangleShape hitbox(sf::Vector2f(xSize, ySize));
    hitbox.setPosition((cellPos.x * CELL_SIZE) - xSize/2, (cellPos.y * CELL_SIZE) - ySize/2);

    if(collideFrame)
        hitbox.setFillColor(sf::Color::Red);
    else
        hitbox.setFillColor(sf::Color::Green);

    target->draw(hitbox);
    target->draw(shape);
}