#include "Chunk.h"
#include <algorithm>

using namespace Segratha;

Chunk::Chunk(int xChunk, int yChunk)
    : xChunk(xChunk), yChunk(yChunk)
{    
    Load();
}

Chunk::~Chunk()
{
    std::cout << "Unloaded" << std::endl;
    Unload(); //unload on destroy
}

void Chunk::Load()
{
    //init Cell array
    for(int x = 0; x < CHUNK_SIZE; x++)
    {
        for(int y = 0; y < CHUNK_SIZE; y++)
        {
            //TODO: grab Cells from disk
            cells[x][y] = Cell(AIR);
            cells[x][y].color = sf::Color(x % 16, y % 16, x % 16, 255);
        }
    }
}

void Chunk::Unload()
{
    //TODO: Write Cells to disk
}

void Chunk::ChunkUpdate(unsigned char cycle)
{
    if(updateCycles <= 0)
        return;

    sf::Vector2i curR1 = rect1;
    sf::Vector2i curR2 = rect2;
    
    curR1.x = curR1.x < 0 ? 0 : curR1.x;
    curR1.y = curR1.y > CHUNK_SIZE - 1 ? CHUNK_SIZE - 1 : curR1.y;

    curR2.x = curR2.x > CHUNK_SIZE - 1 ? CHUNK_SIZE - 1 : curR2.x;
    curR2.y = curR2.y < 0 ? 0 : curR2.y;

    rect1 = sf::Vector2i(-99999, -1);
    rect2 = sf::Vector2i(-1, -1);

    for(int y = curR1.y; y >= curR2.y; --y)
        for(int x = curR1.x; x <= curR2.x; ++x)
        {
            cells[x][y].Update(x + xChunk * CHUNK_SIZE, y + yChunk * CHUNK_SIZE, cycle);
        }
    
    updateCycles -= 1;
}

void Chunk::Touch(int x, int y)
{
    updateCycles = 2;

    int size = 2; //size to touch around pixel

    //calculate the area around size
    int xMin = (x - size);
    int xMax = (x + size);

    int yMin = (y - size);
    int yMax = (y + size);

    //if rect1.x is -99999, we can assume all values are uninitialized
    if(rect1.x == -99999)
    {
        rect1 = sf::Vector2i(xMin, yMax);
        rect2 = sf::Vector2i(xMax, yMin);
        return;
    }

    //otherwise, do funny
    
    if(xMin < rect1.x) //set if x is less
        rect1.x = xMin;
    else if(xMax > rect2.x) //set if x is greater
        rect2.x = xMax; 

    if(yMax > rect1.y) // set if y is greater (increased y is lower, and rect1 is bottom left) 
        rect1.y = yMax;
    if(yMin < rect2.y) //set if y is less
        rect2.y = yMin;
}