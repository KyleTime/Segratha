#include "Component.h"

using namespace Segratha;

Component::Component()
{
}

Component::~Component()
{
}

void Component::SetParent(GameObject* parent)
{
    gameObject = parent;
}

GameObject* Component::GetParent()
{
    return gameObject;
}

void Component::Awake()
{

}

void Component::Update()
{

}

void Component::Draw(sf::RenderWindow* target)
{
    
}