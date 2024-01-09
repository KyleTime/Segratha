#include "PlayerMovement.h"
#include "../GameObject.h"

using namespace Segratha;

void PlayerMovement::Awake()
{
    phys = reinterpret_cast<BasicPhysics*>(gameObject->GetComponent("BasicPhysics"));

    if(phys == nullptr)
    {
        std::cerr << "COMPONENT NOT FOUND!" << std::endl;
    }
}

void PlayerMovement::Update()
{
    HandleJump();
    HandleRun();
}

void PlayerMovement::Draw(sf::RenderWindow* target)
{
    
}

void PlayerMovement::HandleRun()
{
    
}

void PlayerMovement::HandleJump()
{

}