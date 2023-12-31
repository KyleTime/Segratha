#include "ChunkRend.h"
#include "CaveSand.h"

using namespace Segratha;

ChunkRend::ChunkRend()
    : vert(sf::Quads, REND_SIZE * REND_SIZE * 4), transform(sf::Transform::Identity)
{
        for (int i = 0; i < REND_SIZE * REND_SIZE; ++i) {   
        // Get the position and size of the cell
        sf::Vector2f pos((i % REND_SIZE) * CELL_SIZE, (i / REND_SIZE) * CELL_SIZE);

        // Set the position of each vertex
        vert[i * 4 + 0].position = pos;
        vert[i * 4 + 1].position = pos + sf::Vector2f(CELL_SIZE, 0);
        vert[i * 4 + 2].position = pos + sf::Vector2f(CELL_SIZE, CELL_SIZE);
        vert[i * 4 + 3].position = pos + sf::Vector2f(0, CELL_SIZE);
    }
}

void ChunkRend::Bind(int x, int y, Chunk* c)
{
    active = true; //tell the renderchunk that it's active
    
    transform = transform.Identity;
    transform.translate(x * REND_SIZE * CELL_SIZE, y * REND_SIZE * CELL_SIZE); //translate by chunk
    position = sf::Vector2i(x, y);

    bound = c;
}

void ChunkRend::Unbind()
{
    active = false;
    bound = nullptr;
}

void ChunkRend::ChunkDraw(sf::RenderWindow* target)
{
    //THIS DEBUG THINGY MAKES EACH RENDER CHUNK SHOW WHERE IT IS
    /*
    sf::Font font;
    font.loadFromFile("minecrap.ttf");
    sf::Text rendPos("", font, 50);
    rendPos.setString("(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
    rendPos.setPosition(sf::Vector2f(REND_SIZE * CELL_SIZE * position.x, REND_SIZE * CELL_SIZE * position.y));
    target->draw(rendPos);*/

    if(!active)
        return;

    /*
    int startX = ((position.x) % DIVISOR)*REND_SIZE;
    int startY = ((position.y) % DIVISOR)*REND_SIZE;

    if(startX < 0)
        startX += CHUNK_SIZE - 1;

    if(startY < 0)
        startY += CHUNK_SIZE - 1;

    if(startX < 0 || startY < 0)
    {
        return;
    }*/

    //std::cout << "INPUTS: [CHUNK]= " << bound->xChunk << ", " << bound->yChunk << " [POSITION]= " << position.x << ", " << position.y << std::endl;

    int startX = (position.x) * REND_SIZE;
    int startY = (position.y) * REND_SIZE;

    //std::cout << "start1: " << startX << ", " << startY << std::endl;

    startX -= bound->xChunk * CHUNK_SIZE;
    startY -= bound->yChunk * CHUNK_SIZE;

    //std::cout << "start2: " << startX << ", " << startY << std::endl;

    startX %= CHUNK_SIZE;
    startY %= CHUNK_SIZE;

    //std::cout << "start3: " << startX << ", " << startY << std::endl;
    //if(startX < 0 || startY < 0)
    //{
        //std::cout << "FUCK startX: " << startX << ", " << startY << " chunk: " << bound->xChunk << ", " << bound->yChunk << std::endl;
    //    return;
    //}

    if(!bound)
    {
        std::cout << "failed to bind" << std::endl;
        return;
    }

    for(int x = 0; x < REND_SIZE; x++)
        for(int y = 0; y < REND_SIZE; y++)
        {
            int i = x + y * REND_SIZE;

            sf::Color cellColor = (bound->cells[(x + startX)][(y + startY)]).color;

            vert[i * 4 + 0].color = cellColor;
            vert[i * 4 + 1].color = cellColor;
            vert[i * 4 + 2].color = cellColor;
            vert[i * 4 + 3].color = cellColor;
        }

    //set vertex colors
    /*for (int i = 0; i < REND_SIZE * REND_SIZE; ++i)
    {
        sf::Color cellColor = (bound->cells[(i % REND_SIZE)][(i / REND_SIZE)]).color;
        
        vert[i * 4 + 0].color = cellColor;
        vert[i * 4 + 1].color = cellColor;
        vert[i * 4 + 2].color = cellColor;
        vert[i * 4 + 3].color = cellColor;
    }*/

    target->draw(vert, transform);

    /*
    sf::RectangleShape rect(sf::Vector2f(REND_SIZE * CELL_SIZE, REND_SIZE * CELL_SIZE));
    rect.setPosition(sf::Vector2f(CELL_SIZE * REND_SIZE * position.x, CELL_SIZE * REND_SIZE * position.y));
    rect.setOutlineThickness(10);*/
    
    /*
    if(abs(position.x % DIVISOR) == DIVISOR - 1)
        rect.setOutlineColor(sf::Color::White);
    if(position.x < 0 && position.y < 0)
        rect.setOutlineColor(sf::Color::Red);
    else if(position.x < 0)
        rect.setOutlineColor(sf::Color::Yellow);
    else if(position.y < 0)
        rect.setOutlineColor(sf::Color::Magenta);
    else
        rect.setOutlineColor(sf::Color::Green);
    */

   /*
    int color = abs(bound->xChunk) % 3;

    switch(color)
    {
        case 0:
            rect.setOutlineColor(sf::Color::Red);
            break;
        case 1:
            rect.setOutlineColor(sf::Color::Green);
            break;
        case 2:
            rect.setOutlineColor(sf::Color::Cyan);
            break;
    }

    rect.setFillColor(sf::Color::Transparent);
    target->draw(rect);*/


    /*
    //chunk border
    sf::RectangleShape border(sf::Vector2f(REND_SIZE * CELL_SIZE, REND_SIZE * CELL_SIZE));
    border.setPosition(CELL_SIZE * REND_SIZE * bound->xChunk, CELL_SIZE * REND_SIZE * bound->yChunk);
    border.setOutlineThickness(10);
    border.setOutlineColor(sf::Color::Green);
    border.setFillColor(sf::Color::Transparent);
    //target->draw(border);*/

    /*
    sf::Vector2i curR1 = bound->rect1;
    sf::Vector2i curR2 = bound->rect2;
    
    curR1.x = curR1.x < 0 ? 0 : curR1.x;
    curR1.y = curR1.y > REND_SIZE - 1 ? REND_SIZE - 1 : curR1.y;

    curR2.x = curR2.x > REND_SIZE - 1 ? REND_SIZE - 1 : curR2.x;
    curR2.y = curR2.y < 0 ? 0 : curR2.y;

    //dirtyrect
    sf::RectangleShape rect(sf::Vector2f((curR2.x - curR1.x) * CELL_SIZE, (curR1.y - curR2.y) * CELL_SIZE));
    rect.setPosition(curR1.x * CELL_SIZE + REND_SIZE * CELL_SIZE * bound->xChunk, curR2.y * CELL_SIZE + REND_SIZE * CELL_SIZE * bound->yChunk);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(5);
    rect.setOutlineColor(sf::Color::Red);
    target->draw(rect);*/
}