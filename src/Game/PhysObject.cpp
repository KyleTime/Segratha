#include "PhysObject.h"
#include "../CaveSand/KyleTime.hpp"

using namespace Segratha;

PhysObject::PhysObject()
    : GameObject(), velocity(sf::Vector2f()), grounded(false), doGravity(true)
{

}

PhysObject::~PhysObject() {}

bool PhysObject::FullCollisionCheck(CaveSand* sand)
{   
    bool collided = false;

    
    //we first check if we moving down so that we don't do a ground check at an improper time
    if(velocity.y > 0 && CheckGround(sand) || velocity.y < 0 && CheckCeiling(sand))
    {
        velocity.y = 0;
        collided = true;
    }

    if(velocity.x > 0 && CheckRightWall(sand) || velocity.x < 0 && CheckLeftWall(sand))
    {
        velocity.x = 0;
        collided = true;
    }

    if(collided)
    {
        FullCollisionCheck(sand);
    }

    return collided;
}

void PhysObject::Update(CaveSand* sand)
{
    GameObject::Update(sand);

    velocity.y -= GRAVITY_SCALE * KyleTime::DeltaTime();

    position.x += velocity.x * KyleTime::DeltaTime();
    position.y += velocity.y * KyleTime::DeltaTime();

    //std::cout << "VELOCITY! " << velocity.x << ", " << velocity.y << std::endl;
    FullCollisionCheck(sand);
}