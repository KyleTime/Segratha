#include "PlayerItem.h"
#include "../GameObject.h"
#include "../KInput.h"
#include <cmath>

using namespace Segratha;

PlayerItem::PlayerItem(SpriteRenderer* rend, GameObject* player)
{
    this->rend = rend;
    this->player = player;
}

void PlayerItem::Awake()
{

}

void PlayerItem::Update()
{
    MoveToPlayer(sf::Vector2f());
    RotateToMouse(0);
}

void PlayerItem::Draw(sf::RenderWindow* target)
{
    // sf::CircleShape s(10);
    // s.setPosition(gameObject->position);
    // s.setFillColor(sf::Color::Magenta);
    // target->draw(s);
}

std::string PlayerItem::GetComponentID()
{
    return "PlayerItem";
}

void PlayerItem::MoveToPlayer(sf::Vector2f offset)
{
    gameObject->position = player->position + offset;
}

void PlayerItem::RotateToMouse(float radOffset)
{
    sf::Vector2f mouseWorld = KInput::GetMousePositionWorld();

    float rotato = atan2(mouseWorld.y - (player->position.y), mouseWorld.x - (player->position.x));

    rend->rotato = (rotato) * (180.0/3.141592653589793238463);
}