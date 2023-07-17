#include "GameObject.h"

using namespace Segratha;

GameObject::GameObject()
    : position(sf::Vector2f(0, 0)), cellScale(sf::Vector2i(1, 1)), cellPos(sf::Vector2i(0, 0))
{

}

GameObject::~GameObject()
{

}

bool GameObject::CellCollide(CaveSand* sand)
{
    Chunk* chunk = sand->GetChunkCell(cellPos.x - cellScale.x, cellPos.y - cellScale.y); //grab the chunk at the top left of the collider
    for(int x = -cellScale.x; x < cellScale.x; x++)
        for(int y = -cellScale.y; y < cellScale.y; y++)
        {
            //what chunk is our current position in? (used to check if we need to shift the chunk)
            sf::Vector2i curChunk = sand->CellToChunkPos(cellPos + sf::Vector2i(x, y));


            //have we crossed into a new chunk?
            if(!chunk || curChunk.x != chunk->xChunk || curChunk.y != chunk->yChunk)
            {
                chunk = sand->GetChunk(curChunk.x, curChunk.y);
            }

            //if we don't got a chunk rn, move on
            if(!chunk)
                continue;

            //now we should be free to check the funny chunk
            
            //is there a solid?
            sf::Vector2i rel = sand->RelCellPos(cellPos + sf::Vector2i(x, y));

            if(!chunk->cells[rel.x][rel.y].isAir())
            {
                return true;
            }
        }
    
    return false;
}

void GameObject::Update(CaveSand* sand)
{
    cellPos = sand->WorldToCell(position); //update cellPos

    collideFrame = CellCollide(sand);
}