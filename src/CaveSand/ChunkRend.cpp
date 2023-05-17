#include "ChunkRend.h"

#define RENDCHUNK_SIZE (CHUNK_SIZE / REND_FACTOR)

using namespace Segratha;

ChunkRend::ChunkRend()
    : vert(sf::Quads, RENDCHUNK_SIZE * RENDCHUNK_SIZE * 4)
{
}

void ChunkRend::Bind(Chunk* c, sf::Vector2f rendPos, int startPos)
{
    for (int i = 0; i < RENDCHUNK_SIZE * RENDCHUNK_SIZE; ++i) {   
        
        // Get the position and size of the cell
        sf::Vector2f pos((i % RENDCHUNK_SIZE) * CELL_SIZE, (i / RENDCHUNK_SIZE) * CELL_SIZE); 
        pos += rendPos;

        // Set the position of each vertex
        vert[i * 4 + 0].position = pos;
        vert[i * 4 + 1].position = pos + sf::Vector2f(CELL_SIZE, 0);
        vert[i * 4 + 2].position = pos + sf::Vector2f(CELL_SIZE, CELL_SIZE);
        vert[i * 4 + 3].position = pos + sf::Vector2f(0, CELL_SIZE);
    }

    rendStart = startPos;
    bound = c;
}

void ChunkRend::Unbind()
{
    bound = nullptr;
}

void ChunkRend::ChunkDraw(sf::RenderWindow* target)
{
    if(!bound)
        return;

    //set vertex colors
    for (int i = rendStart; i < RENDCHUNK_SIZE * RENDCHUNK_SIZE; ++i)
    {
        sf::Color cellColor = (bound->cells[(i % RENDCHUNK_SIZE)][(i / RENDCHUNK_SIZE)]).color;
        
        vert[i * 4 + 0].color = cellColor;
        vert[i * 4 + 1].color = cellColor;
        vert[i * 4 + 2].color = cellColor;
        vert[i * 4 + 3].color = cellColor;
    }

    target->draw(vert);

    /*
    //chunk border
    sf::RectangleShape border(sf::Vector2f(RENDCHUNK_SIZE * CELL_SIZE, RENDCHUNK_SIZE * CELL_SIZE));
    border.setPosition(CELL_SIZE * RENDCHUNK_SIZE * bound->xChunk, CELL_SIZE * RENDCHUNK_SIZE * bound->yChunk);
    border.setOutlineThickness(10);
    border.setOutlineColor(sf::Color::Green);
    border.setFillColor(sf::Color::Transparent);
    //target->draw(border);*/

    sf::Vector2i curR1 = bound->rect1;
    sf::Vector2i curR2 = bound->rect2;
    
    curR1.x = curR1.x < 0 ? 0 : curR1.x;
    curR1.y = curR1.y > RENDCHUNK_SIZE - 1 ? RENDCHUNK_SIZE - 1 : curR1.y;

    curR2.x = curR2.x > RENDCHUNK_SIZE - 1 ? RENDCHUNK_SIZE - 1 : curR2.x;
    curR2.y = curR2.y < 0 ? 0 : curR2.y;

    //dirtyrect
    sf::RectangleShape rect(sf::Vector2f((curR2.x - curR1.x) * CELL_SIZE, (curR1.y - curR2.y) * CELL_SIZE));
    rect.setPosition(curR1.x * CELL_SIZE + RENDCHUNK_SIZE * CELL_SIZE * bound->xChunk, curR2.y * CELL_SIZE + RENDCHUNK_SIZE * CELL_SIZE * bound->yChunk);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(5);
    rect.setOutlineColor(sf::Color::Red);
    target->draw(rect);
}