#include "GameObject.h"

using namespace Segratha;

GameObject::GameObject()
    : components(std::vector<Component*>())
{
    position = sf::Vector2f(0, 0);
}

GameObject::~GameObject()
{
    //make sure we clean up our mess lol
    for(int i = 0; i < components.size(); i++)
    {
        delete components[i];
    }
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

void GameObject::Die()
{
    //marked for deletion
    garbage = true;
}

Component* GameObject::AddComponent(Component* Component)
{
    Component->SetParent(this);
    components.push_back(Component);

    return Component;
}

Component* GameObject::GetComponent(std::string componentID)
{
    for(int i = 0; i < components.size(); i++)
    {
        if(components[i]->GetComponentID() == componentID)
            return components[i];
    }

    return nullptr;
}