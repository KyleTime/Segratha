#include "BasicPhysics.h"
#include "../GameObject.h"
#include <algorithm>

using namespace Segratha;

void BasicPhysics::Awake()
{
    velocity = sf::Vector2f();
    state = OPEN_AIR;
    grounded = false;
}

void BasicPhysics::Update()
{
    StateUpdate();
    GravityUpdate();
    Drag();
    MoveSegmented();
}

void BasicPhysics::Draw(sf::RenderWindow* target)
{
    CellCollider::Draw(target);
}

std::string BasicPhysics::GetComponentID()
{
    return "BasicPhysics";
}

bool BasicPhysics::FullCollisionCheck()
{   
    CaveSand* sand = CaveSand::GetInstance();

    bool collided = false;
    grounded = false;
    //we first check if we moving down so that we don't do a ground 
    if(velocity.y > 0)
    {
        if(CheckGround(sand))
        {
            grounded = true;
            velocity.y = 0;
            collided = true;
        }
    }
    else if(velocity.y < 0)
    {
        //bool g = CheckGround(sand); //check if we grounded
        float posY = gameObject->position.y; //store the current player position
        if(CheckCeiling(sand)) //did we bonk?
        {
            //we also gotta check ground to make sure we ain't stuck
            if(CheckGround(sand))
            {
                //STUCK AS FUCK
                gameObject->position.y = posY;
            }
            //collision
            velocity.y = 0;
            collided = true;
        }
    }
    if(velocity.x > 0 && CheckRightWall(sand) || velocity.x < 0 && CheckLeftWall(sand))
    {
        velocity.x = 0;
        collided = true;
    }
    return collided;
}
void BasicPhysics::StateUpdate()
{
    CaveSand* sand = CaveSand::GetInstance();

    cellTypeList list = CellCollide(sand);
    if(list.list[LIQUID] > list.size*0.75f)
    {
        state = SWIMMING;
    }
    else
    {
        state = OPEN_AIR;
    }
}
void BasicPhysics::GravityUpdate()
{
    velocity.y -= GRAVITY_SCALE * KyleTime::DeltaTime();
    velocity.y = std::min(velocity.y, TERMINAL_VELOCITY); //cap grav
}
void BasicPhysics::Drag()
{
    float XDRAG = 0;
    float YDRAG = 0;
    switch(state)
    {
        case SWIMMING:
            XDRAG = 2;
            YDRAG = 2;
            break;
        default:
            break;
    }
    velocity.x -= velocity.x * XDRAG * KyleTime::DeltaTime();
    velocity.y -= velocity.y * YDRAG * KyleTime::DeltaTime();
}
void BasicPhysics::MoveSegmented()
{
    CaveSand* sand = CaveSand::GetInstance();

    //these two numbers make it so that our speed values don't have 
    //speed is relative to Cell size
    float transVelX = velocity.x * CELL_SIZE;
    float transVelY = velocity.y * CELL_SIZE;
    if(state == SWIMMING)
    {
        transVelY *= 0.6f;
        transVelX *= 0.6f;
    }
    //DIVIDE STUFF BY DIV
    //note: I forgot what this does exactly
    int div = std::max((int)std::abs(std::max(transVelX, transVelY)) / 400, 4);
    //DIVIDE THESE BY 4 TO GET HOW MUCH THE PLAYER MOVES IN A SINGLE
    float velXF = transVelX / div;
    float velYF = transVelY / div;
    //LOOP FOUR TIMES, ONE, TWO, THREE, FOUR
    //THEN TAKE IT ARRRROOOOOOUNNNDDDDDD TOWNNNNNN
    //WEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    //WEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    //STOP IN A VALID CELL POSITION DON'T FORGET IT!
    for(int x = 0; x < div; x++)
    {
        gameObject->position.x += velXF * KyleTime::DeltaTime();
        gameObject->position.y += velYF * KyleTime::DeltaTime();
        if(FullCollisionCheck())
        {
            if(velocity.x == 0)
                velXF = 0;
            if(velocity.y == 0)
                velYF = 0;
            if(velXF == 0 && velYF == 0)
                break;
        }
    }
}