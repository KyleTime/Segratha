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

    return collided;
}

void PhysObject::Update(CaveSand* sand)
{
    GameObject::Update(sand);

    velocity.y -= GRAVITY_SCALE * KyleTime::DeltaTime();

    //these two numbers make it so that our speed values don't have to be so huge
    //speed is relative to Cell size
    float transVelX = velocity.x * CELL_SIZE;
    float transVelY = velocity.y * CELL_SIZE;

    //DIVIDE STUFF BY DIV
    int div = std::max((int)std::abs(std::max(transVelX, transVelY)) / 400, 4);

    //DIVIDE THESE BY 4 TO GET HOW MUCH THE PLAYER MOVES IN A SINGLE LOOP
    float velXF = transVelX / div;
    float velYF = transVelY / div;

    //LOOP FOUR TIMES, ONE, TWO, THREE, FOUR
    //THEN TAKE IT ARRRROOOOOOUNNNDDDDDD TOWNNNNNN
    //WEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    //WEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    //STOP IN A VALID CELL POSITION DON'T FORGET IT!
    for(int x = 0; x < div; x++)
    {
        position.x += velXF * KyleTime::DeltaTime();
        position.y += velYF * KyleTime::DeltaTime();

        if(FullCollisionCheck(sand))
        {
            if(velocity.x == 0)
                velXF = 0;
            if(velocity.y == 0)
                velYF = 0;

            if(velXF == 0 && velYF == 0)
                break;
        }
    }

    // position.x += velocity.x * KyleTime::DeltaTime();
    // position.y += velocity.y * KyleTime::DeltaTime();

    // FullCollisionCheck(sand);
}