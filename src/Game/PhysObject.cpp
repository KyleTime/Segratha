// #include "PhysObject.h"
// #include "../CaveSand/KyleTime.hpp"

// using namespace Segratha;

// PhysObject::PhysObject()
//     : GameObject(), velocity(sf::Vector2f()), grounded(false), state(OPEN_AIR)
// {

// }

// PhysObject::~PhysObject() {}

// bool PhysObject::FullCollisionCheck(CaveSand* sand)
// {   
//     bool collided = false;
//     grounded = false;

//     //we first check if we moving down so that we don't do a ground check at an improper time
//     if(velocity.y > 0)
//     {
//         if(CheckGround(sand))
//         {
//             grounded = true;
//             velocity.y = 0;
//             collided = true;
//         }
//     }
//     else if(velocity.y < 0)
//     {
//         //bool g = CheckGround(sand); //check if we grounded

//         float posY = position.y; //store the current player position to revert to if we get stuck

//         if(CheckCeiling(sand)) //did we bonk?
//         {
//             //we also gotta check ground to make sure we ain't stuck
//             if(CheckGround(sand))
//             {
//                 //STUCK AS FUCK
//                 position.y = posY;
//             }

//             //collision
//             velocity.y = 0;
//             collided = true;
//         }
//     }

//     if(velocity.x > 0 && CheckRightWall(sand) || velocity.x < 0 && CheckLeftWall(sand))
//     {
//         velocity.x = 0;
//         collided = true;
//     }

//     return collided;
// }

// void PhysObject::StateUpdate(CaveSand* sand)
// {
//     cellTypeList list = CellCollide(sand);

//     if(list.list[LIQUID] > list.size*0.75f)
//     {
//         state = SWIMMING;
//     }
//     else
//     {
//         state = OPEN_AIR;
//     }
// }

// void PhysObject::GravityUpdate()
// {
//     velocity.y -= GRAVITY_SCALE * KyleTime::DeltaTime();
//     velocity.y = std::min(velocity.y, TERMINAL_VELOCITY); //cap gravity at TERMINAL_VELOCITY
// }

// void PhysObject::Drag()
// {
//     float XDRAG = 0;
//     float YDRAG = 0;

//     switch(state)
//     {
//         case SWIMMING:
//             XDRAG = 2;
//             YDRAG = 2;
//             break;
//         default:
//             break;
//     }

//     velocity.x -= velocity.x * XDRAG * KyleTime::DeltaTime();
//     velocity.y -= velocity.y * YDRAG * KyleTime::DeltaTime();
// }

// void PhysObject::MoveSegmented(CaveSand* sand)
// {
//     //these two numbers make it so that our speed values don't have to be so huge
//     //speed is relative to Cell size
//     float transVelX = velocity.x * CELL_SIZE;
//     float transVelY = velocity.y * CELL_SIZE;

//     if(state == SWIMMING)
//     {
//         transVelY *= 0.6f;
//         transVelX *= 0.6f;
//     }

//     //DIVIDE STUFF BY DIV
//     //note: I forgot what this does exactly
//     int div = std::max((int)std::abs(std::max(transVelX, transVelY)) / 400, 4);

//     //DIVIDE THESE BY 4 TO GET HOW MUCH THE PLAYER MOVES IN A SINGLE LOOP
//     float velXF = transVelX / div;
//     float velYF = transVelY / div;

//     //LOOP FOUR TIMES, ONE, TWO, THREE, FOUR
//     //THEN TAKE IT ARRRROOOOOOUNNNDDDDDD TOWNNNNNN
//     //WEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
//     //WEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
//     //STOP IN A VALID CELL POSITION DON'T FORGET IT!
//     for(int x = 0; x < div; x++)
//     {
//         position.x += velXF * KyleTime::DeltaTime();
//         position.y += velYF * KyleTime::DeltaTime();

//         if(FullCollisionCheck(sand))
//         {
//             if(velocity.x == 0)
//                 velXF = 0;
//             if(velocity.y == 0)
//                 velYF = 0;

//             if(velXF == 0 && velYF == 0)
//                 break;
//         }
//     }

// }

// void PhysObject::Update(CaveSand* sand)
// {
//     GameObject::Update();

//     StateUpdate(sand);

//     GravityUpdate();

//     Drag();

//     MoveSegmented(sand);
// }