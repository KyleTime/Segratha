#include "PlayerCore.h"
#include "SFML/Graphics.hpp"
#include "KyleTime.hpp"

using namespace Segratha;

PlayerCore::PlayerCore()
    : position(0, 0)
{
}

PlayerCore::~PlayerCore() {}

void PlayerCore::Update()
{
    float speed = 1000;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        position.y -= speed * KyleTime::DeltaTime();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        position.y += speed * KyleTime::DeltaTime();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        position.x += speed * KyleTime::DeltaTime();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        position.x -= speed * KyleTime::DeltaTime();
    }
}

void PlayerCore::Draw(sf::RenderWindow* target)
{
    sf::RectangleShape shape(sf::Vector2f(20, 20));
    shape.setPosition(position.x - 10, position.y + 10);

    target->draw(shape);
}