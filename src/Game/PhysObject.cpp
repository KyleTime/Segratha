#include "PhysObject.h"
#include "../CaveSand/KyleTime.hpp"

using namespace Segratha;

PhysObject::PhysObject()
    : GameObject(), velocity(sf::Vector2f()), grounded(false), doGravity(true)
{

}

PhysObject::~PhysObject() {}

void PhysObject::Update(CaveSand* sand)
{
    GameObject::Update(sand);

    //use to store point of collision in cell coords
    sf::Vector2i point;

    //update x position
    position.x += velocity.x * KyleTime::DeltaTime();
    
    //check if we're colliding
    if(CellCollide(sand, point, false))
    {
        //figure out what direction we're moving in
        int mod = 1; 
        if(velocity.x < 0)
            mod = -1;

        //no more velocity because we ballin
        velocity.x = 0;

        //update our position so that we're hopefully not in the wall
        position.x = (point.x + mod*(cellScale.x + 0.5f)) * CELL_SIZE;
    }

    //update gravity if necessary
    if(doGravity)
        velocity.y += GRAVITY_SCALE * KyleTime::DeltaTime();
    
    grounded = false; //reset grounded

    //update y position
    position.y -= velocity.y * KyleTime::DeltaTime();

    //check the collision again, reusing point because we're already done with the data it held
    if(CellCollide(sand, point, true))
    {
        //figure out what direction we're moving in
        int mod = 1; 
        if(velocity.y < 0)
            mod = -1;

        //no mas uppy or downy
        velocity.y = 0;

        //update our position so that we're hopefully not in the wall
        position.y = (point.y + mod*(cellScale.y - 0.5f)) * CELL_SIZE;

        //feet moment
        //if(abs(point.y - cellPos.y) > cellScale.y)
        //    position.y -= CELL_SIZE;

        if(mod < 0)
            grounded = true;
    }
}