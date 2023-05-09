#include "Chunk.h"
#include <algorithm>

using namespace Segratha;

Chunk::Chunk(int xChunk, int yChunk)
    : vert(sf::Quads, CHUNK_SIZE * CHUNK_SIZE * 4), xChunk(xChunk), yChunk(yChunk)
{
    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; ++i) {   
        
        // Get the position and size of the cell
        sf::Vector2f pos((i % CHUNK_SIZE) * CELL_SIZE, (i / CHUNK_SIZE) * CELL_SIZE);
        pos += sf::Vector2f(xChunk * CELL_SIZE * CHUNK_SIZE, yChunk * CELL_SIZE * CHUNK_SIZE);
    
        // Set the position of each vertex
        vert[i * 4 + 0].position = pos;
        vert[i * 4 + 1].position = pos + sf::Vector2f(CELL_SIZE, 0);
        vert[i * 4 + 2].position = pos + sf::Vector2f(CELL_SIZE, CELL_SIZE);
        vert[i * 4 + 3].position = pos + sf::Vector2f(0, CELL_SIZE);
    }
    
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

void Chunk::ChunkDraw(sf::RenderWindow* target)
{
    //set vertex colors
    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; ++i)
    {
        sf::Color cellColor = (cells[(i % CHUNK_SIZE)][(i / CHUNK_SIZE)]).color;
        
        vert[i * 4 + 0].color = cellColor;
        vert[i * 4 + 1].color = cellColor;
        vert[i * 4 + 2].color = cellColor;
        vert[i * 4 + 3].color = cellColor;
    }

    target->draw(vert);
    /*
    //chunk border
    sf::RectangleShape border(sf::Vector2f(CHUNK_SIZE * CELL_SIZE, CHUNK_SIZE * CELL_SIZE));
    border.setPosition(CELL_SIZE * CHUNK_SIZE * xChunk, CELL_SIZE * CHUNK_SIZE * yChunk);
    border.setOutlineThickness(10);
    border.setOutlineColor(sf::Color::Green);
    border.setFillColor(sf::Color::Transparent);
    //target->draw(border);*/

    sf::Vector2i curR1 = rect1;
    sf::Vector2i curR2 = rect2;
    
    curR1.x = curR1.x < 0 ? 0 : curR1.x;
    curR1.y = curR1.y > CHUNK_SIZE - 1 ? CHUNK_SIZE - 1 : curR1.y;

    curR2.x = curR2.x > CHUNK_SIZE - 1 ? CHUNK_SIZE - 1 : curR2.x;
    curR2.y = curR2.y < 0 ? 0 : curR2.y;

    //dirtyrect
    sf::RectangleShape rect(sf::Vector2f((curR2.x - curR1.x) * CELL_SIZE, (curR1.y - curR2.y) * CELL_SIZE));
    rect.setPosition(curR1.x * CELL_SIZE + CHUNK_SIZE * CELL_SIZE * xChunk, curR2.y * CELL_SIZE + CHUNK_SIZE * CELL_SIZE * yChunk);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(5);
    rect.setOutlineColor(sf::Color::Red);
    target->draw(rect);
}

void Chunk::Touch(int x, int y)
{
    updateCycles = 2;

    int size = 2; //size to touch around pixel

    if(x % CHUNK_SIZE != x || y % CHUNK_SIZE != y)
        std::cout << "TOUCHITY FUCK FUCK FUCK FUCK" << std::endl;

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