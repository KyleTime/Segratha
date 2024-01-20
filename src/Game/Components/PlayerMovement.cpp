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
    grounded = phys->grounded || phys->CheckIfGrounded();

    HandleJump();
    HandleRun();
}

void PlayerMovement::Draw(sf::RenderWindow* target)
{
    sf::RectangleShape center(sf::Vector2f(6, 6));
    center.setPosition(gameObject->position.x - 3, gameObject->position.y - 3);
    
    if(grounded)
        center.setFillColor(sf::Color::Blue);
    else
        center.setFillColor(sf::Color::Magenta);
}

void PlayerMovement::HandleRun()
{
    int input = 0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        input += 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        input -= 1;
    }

    if(input != 0)
    {
        Accelerate(input);
    }
    else
    {
        Decelerate(1);
    }
}

void PlayerMovement::HandleJump()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && grounded)
        phys->velocity.y = -jumpPower;
}

void PlayerMovement::Accelerate(int input)
{
    //if holding the direction you're moving, and you're moving wayyyyy too fast, decelerate at half the rate
    if((input == 1 && phys->velocity.x > maxSpeed || input == -1 && phys->velocity.x < -maxSpeed))
    {
        Decelerate(0.5f);
        return;
    }

    //if we not on the ground, modfiy the velocity by the airMod
    float mod = grounded ? 1 : airMod;

    phys->velocity.x += (input * KyleTime::DeltaTime() * mod) / (acceleration / maxSpeed); 

    //cap speed at max speed while moving normally
    if(input == 1)
    {
        phys->velocity.x = phys->velocity.x > maxSpeed ? maxSpeed : phys->velocity.x;
    }
    else if(input == -1)
    {
        phys->velocity.x = phys->velocity.x < -maxSpeed ? -maxSpeed : phys->velocity.x;
    }
}

void PlayerMovement::Decelerate(float mul)
{
    if(phys->velocity.x == 0)
        return;

    //these two lines basically say:
    //if we not grounded, use airMod
    //if we grounded, use 1, but if we moving too fast too, then go to 2
    float mod = grounded ? 1 : airMod;
    mod = mod == 1 && abs(phys->velocity.x) > maxSpeed ? 2 : 1;

    mod *= mul;

    float tSpeed = (KyleTime::DeltaTime() * mod * (phys->velocity.x > 0 ? 1 : -1)) / (acceleration / maxSpeed);

    if(abs(tSpeed) < abs(phys->velocity.x))
    {
        phys->velocity.x -= tSpeed;
    }
    else
    {
        phys->velocity.x = 0;
    }
}