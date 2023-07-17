#include "PlayerCore.h"
#include "CaveSand/KyleTime.hpp"
#include "CaveSand/Camera.h"

#include <iostream>

using namespace Segratha;

PlayerCore::PlayerCore()
    : GameObject()
{
    cellScale = sf::Vector2i(3, 3);
}

PlayerCore::~PlayerCore() {}

void PlayerCore::Update(CaveSand* sand)
{
    GameObject::Update(sand);

    float speed = 1000;

    float mod = 1;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
        mod = 5;
    else
        mod = 1;
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        position.y -= speed * mod * KyleTime::DeltaTime();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        position.y += speed * mod * KyleTime::DeltaTime();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        position.x += speed * mod * KyleTime::DeltaTime();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        position.x -= speed * mod * KyleTime::DeltaTime();
    }

    CAMERA::ChangeCenter(position);
}

void PlayerCore::Draw(sf::RenderWindow* target)
{
    sf::RectangleShape shape(sf::Vector2f(20, 20));
    shape.setPosition(position.x, position.y);

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