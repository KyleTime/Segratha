#include "GameObject.h"

using namespace Segratha;

GameObject::GameObject()
    : components(std::vector<Component*>())
{
}

GameObject::~GameObject()
{

}

void GameObject::Awake()
{
    for(int i = 0; i < components.size(); i++)
    {
        components[i]->Awake();
    }
}

void GameObject::Update()
{
    static bool awakened = false;

    if(!awakened)
    {
        awakened = true;
        Awake();
    }


    for(int i = 0; i < components.size(); i++)
    {
        components[i]->Update();
    }

    //collideFrame = CellCollide(sand); //update collideFrame
}

void GameObject::Draw(sf::RenderWindow* target)
{
    for(int i = 0; i < components.size(); i++)
    {
        components[i]->Draw(target);
    }
}

Component* GameObject::AddComponent(Component* Component)
{
    Component->SetParent(this);
    components.push_back(Component);

    return Component;
}

