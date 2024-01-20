#include "CellCollider.h"
#include "../../CaveSand/CaveSand.h"
#include "../../Game/GameObject.h"

using namespace Segratha;

/// @brief Create a collider on the object with a size of "scale"
/// @param scale Represents the number of cells out in every direction that the collider spans
CellCollider::CellCollider(sf::Vector2i scale)
{
    cellScale = scale;
}

void CellCollider::Awake()
{

}

void CellCollider::Update()
{

}

void CellCollider::Draw(sf::RenderWindow* target)
{
    //draws a white dot at the player's center
    sf::RectangleShape shape(sf::Vector2f(20, 20));
    shape.setPosition(gameObject->position.x - 10, gameObject->position.y - 10);

    //draws a box that represents the player's body
    float xSize = cellScale.x * CELL_SIZE * 2;
    float ySize = cellScale.y * CELL_SIZE * 2;
    sf::RectangleShape hitbox(sf::Vector2f(xSize, ySize));
    hitbox.setPosition((gameObject->cellPos.x * CELL_SIZE) - xSize/2, (gameObject->cellPos.y * CELL_SIZE) - ySize/2);
    hitbox.setFillColor(sf::Color::Magenta);

    target->draw(hitbox);
    target->draw(shape);
} 

std::string CellCollider::GetComponentID()
{
    return "CellCollider";
}

CellCollider::cellTypeList CellCollider::CellCollide(CaveSand* sand)
{
    gameObject->cellPos = sand->WorldToCell(gameObject->position); //update cellPos

    cellTypeList list;

    for(int i = 0; i < cell_type::Count; i++)
    {
        list.list[i] = 0;
    }

    list.size = 0;

    Chunk* chunk = sand->GetChunkCell(gameObject->cellPos.x - cellScale.x, gameObject->cellPos.y - cellScale.y); //grab the chunk at the top left of the collider
    for(int x = -cellScale.x; x < cellScale.x; x++)
        for(int y = -cellScale.y; y < cellScale.y + 1; y++)
        {
            list.size++; //keep track of how many total cells we check

            //what chunk is our current position in? (used to check if we need to shift the chunk)
            sf::Vector2i curChunk = sand->CellToChunkPos(gameObject->cellPos + sf::Vector2i(x, y));

            //have we crossed into a new chunk?
            if(!chunk || curChunk.x != chunk->xChunk || curChunk.y != chunk->yChunk)
            {
                chunk = sand->GetChunk(curChunk.x, curChunk.y);
            }

            //if we don't got a chunk rn, move on
            if(!chunk)
                continue;

            //now we should be free to check the funny chunk
            
            //get the relative position of our thingy
            sf::Vector2i rel = sand->RelCellPos(gameObject->cellPos + sf::Vector2i(x, y));

            //does it exist?
            if(!chunk->cells[rel.x][rel.y].IsAir())
            {
                Cell c = chunk->cells[rel.x][rel.y];
                cell_type t = c.GetState();
                if(t != c.type) //is this shit not generic?
                    list.list[chunk->cells[rel.x][rel.y].type]++; //we add one to the index of the type (this will tell us how many we encountered)

                list.list[t]++; //add to collisions with generic states
            }
            
        }
    
    return list;
}

// bool GameObject::CellCollide(CaveSand* sand, sf::Vector2i& point, bool feet)
// {
//     cellPos = sand->WorldToCell(position); //update cellPos

//     Chunk* chunk = sand->GetChunkCell(cellPos.x - cellScale.x, cellPos.y - cellScale.y); //grab the chunk at the top left of the collider
//     for(int x = -cellScale.x; x < cellScale.x; x++)
//         for(int y = -cellScale.y; y < cellScale.y + 1; y++)
//         {
//             if(y == cellScale.y + 1 && !feet)
//                 continue;

//             //what chunk is our current position in? (used to check if we need to shift the chunk)
//             sf::Vector2i curChunk = sand->CellToChunkPos(cellPos + sf::Vector2i(x, y));

//             //have we crossed into a new chunk?
//             if(!chunk || curChunk.x != chunk->xChunk || curChunk.y != chunk->yChunk)
//             {
//                 chunk = sand->GetChunk(curChunk.x, curChunk.y);
//             }

//             //if we don't got a chunk rn, move on
//             if(!chunk)
//                 continue;

//             //now we should be free to check the funny chunk
            
//             //is there a solid?
//             sf::Vector2i rel = sand->RelCellPos(cellPos + sf::Vector2i(x, y));

//             if(!chunk->cells[rel.x][rel.y].isAir())
//             {
//                 point = cellPos + sf::Vector2i(x, y);

//                 return true;
//             }
//         }
    
//     return false;
// }

bool CellCollider::CheckGround(CaveSand* sand)
{
    //this function will take the world position of the feet and see if there's anything there in the cell position

    //grab world position below the character
    float feetPos = gameObject->position.y + (cellScale.y) * CELL_SIZE;

    //the cell position of the cell below the character
    sf::Vector2i footCell = sand->WorldToCell(sf::Vector2f(gameObject->position.x, feetPos));

    //for whatever reason, the cell position is a lil off when it goes negative, lol
    //I'm too afraid to mess with GetCellAt(), so this will be my fix
    // if(cellPos.y < 0)
    //     footCell.y += 1;

    //is the cell beneath us solid?
    bool solid = false;

    //SWEEP FROM LEFT TO RIGHT!
    for(int x = -cellScale.x; x < cellScale.x && !solid; x++)
    {
        Cell* c = sand->GetCellAt(footCell.x + x, footCell.y);
        solid = c != nullptr && c->IsSolid();
    }

    //if we hit something, let's try and steppy
    if(solid)
    {
        //move feetPos into the player
        feetPos += CELL_SIZE;

        bool steppy = false;

        for(int y = -cellScale.y; y < cellScale.y && !steppy; y++)
        {
            Cell* c1 = sand->GetCellAt(gameObject->cellPos.x + cellScale.x - 1, gameObject->cellPos.y + y);
            Cell* c2 = sand->GetCellAt(gameObject->cellPos.x - cellScale.x + 1, gameObject->cellPos.y + y);
            steppy = c1 != nullptr && c1->IsSolid() || c2 != nullptr && c2->IsSolid();

            if(steppy && cellScale.y - y > STEPPY_RANGE)
            {
                steppy = false;
                break;
            }
        }

        //no steppy :(
        if(!steppy)
        {
            feetPos -= CELL_SIZE;
        }
    }

    //DEBUG THAT SHOWS THE SELECTED CELL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // sf::RectangleShape cellLook(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    // cellLook.setPosition(footCell.x * CELL_SIZE, footCell.y * CELL_SIZE);

    // if(!solid)
    //     cellLook.setFillColor(sf::Color::Transparent);
    // else
    //     cellLook.setFillColor(sf::Color::Blue);
    
    // cellLook.setOutlineThickness(5);
    // cellLook.setOutlineColor(sf::Color::Red);

    // target->draw(cellLook);
    //END DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    if(solid)
    {
        //we need to figure out the depth at which we are collided

        //calculate the distance between the cell and the feet of the player (world)
        float depth = (footCell.y * CELL_SIZE) - feetPos;
        //add the difference to the y position
        gameObject->position.y += depth + CELL_SIZE * 0.5f; //NOTE: I have no idea why the half cell addition is needed, it just is
        
        return true;
    }

    return false;
}

bool CellCollider::CheckRightWall(CaveSand* sand)
{
    //grab world position to the right of our character
    float rightPos = gameObject->position.x + (cellScale.x) * CELL_SIZE;

    //the cell position of the right cell, we'll use this to find other cells we're interested in, teehee
    sf::Vector2i rightCell = sand->WorldToCell(sf::Vector2f(rightPos, gameObject->position.y));

    //is the cell to the side of us solid?
    bool solid = false;

    //SWEEP FROM TOP TO BOTTOM! (It's important that we raise the bottom collider by steppy so we don't get weird shit goin on)
    for(int y = -cellScale.y + 1; y <= cellScale.y - STEPPY_RANGE && !solid; y++)
    {
        //checks the cell for being solid (also for whether it exists)
        Cell* c = sand->GetCellAt(rightCell.x, rightCell.y + y);
        solid = c != nullptr && c->IsSolid();
    }

    //did we fucking hit a goofy ass wall? DID WE?
    if(solid) //<--- Clueless
    {
        //depth calculations!
        float depth = (rightCell.x * CELL_SIZE) - rightPos;
        gameObject->position.x += depth + CELL_SIZE * 0.5f;
        return true;
    }

    return false;
}

bool CellCollider::CheckLeftWall(CaveSand* sand)
{
    //grab world position to the right of our character
    float leftPos = gameObject->position.x - (cellScale.x + 1) * CELL_SIZE;

    //the cell position of the right cell, we'll use this to find other cells we're interested in, teehee
    sf::Vector2i leftCell = sand->WorldToCell(sf::Vector2f(leftPos, gameObject->position.y));

    //is the cell to the side of us solid?
    bool solid = false;

    //SWEEP FROM TOP TO BOTTOM! (It's important that we raise the bottom collider by steppy so we don't get weird shit goin on)
    for(int y = -cellScale.y + 1; y <= cellScale.y - STEPPY_RANGE && !solid; y++)
    {
        //checks the cell for existing and whether it's solid
        Cell* c = sand->GetCellAt(leftCell.x, leftCell.y + y);
        solid = c != nullptr && c->IsSolid();
    }

    //did we fucking hit a goofy ass wall? DID WE?
    if(solid) //<--- Clueless
    {
        //depth calculations!
        float depth = (leftCell.x * CELL_SIZE) - leftPos;
        gameObject->position.x += depth + CELL_SIZE * 0.5f;
        return true;
    }

    return false;
}

bool CellCollider::CheckCeiling(CaveSand* sand)
{
    //grab world position above the character
    float headPos = gameObject->position.y - (cellScale.y) * CELL_SIZE;

    //the cell position of the cell above the character
    sf::Vector2i headCell = sand->WorldToCell(sf::Vector2f(gameObject->position.x, headPos));

    //is the cell above us solid?
    bool solid = false;

    //SWEEP FROM LEFT TO RIGHT!
    for(int x = -cellScale.x; x < cellScale.x && !solid; x++)
    {
        Cell* c = sand->GetCellAt(headCell.x + x, headCell.y);
        solid = c != nullptr && c->IsSolid();
    }

    //DEBUG THAT SHOWS THE SELECTED CELL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // sf::RectangleShape cellLook(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    // cellLook.setPosition(footCell.x * CELL_SIZE, footCell.y * CELL_SIZE);

    // if(!solid)
    //     cellLook.setFillColor(sf::Color::Transparent);
    // else
    //     cellLook.setFillColor(sf::Color::Blue);
    
    // cellLook.setOutlineThickness(5);
    // cellLook.setOutlineColor(sf::Color::Red);

    // target->draw(cellLook);
    //END DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    if(solid)
    {
        //we need to figure out the depth at which we are collided

        //calculate the distance between the cell and the head of the player (world)
        float depth = (headCell.y * CELL_SIZE) - headPos;
        //add the difference to the y position
        gameObject->position.y += depth + CELL_SIZE; //NOTE: I have no idea why the half cell addition is needed, it just is
        
        return true;
    }

    return false;
}

bool CellCollider::CheckIfGrounded()
{
    CaveSand* cave = CaveSand::GetInstance();

    int yP = gameObject->cellPos.y + cellScale.y + 0; //get the layer just below the feets

    bool solid = false;

    for(int x = -cellScale.x; x < cellScale.x && !solid; x++)
    {
        Cell* c = cave->GetCellAt(gameObject->cellPos.x + x, yP);
        solid = c != nullptr && c->IsSolid();
    } 

    return solid;
}