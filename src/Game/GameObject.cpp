#include "GameObject.h"

using namespace Segratha;

GameObject::GameObject()
    : components(std::vector<Component*>())
{
    position = sf::Vector2f(0, 0);
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
    static CaveSand* sand = CaveSand::GetInstance();

    cellPos = sand->WorldToCell(position); //update cellPos

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

