#include "CaveSand.h"

using namespace Segratha;

Cell::Cell()
    : color(sf::Color::Magenta), cycle(0), type(AIR)
{

}

Cell::Cell(cell_type type)
    : cycle(0), type(type)
{
    static char offset = 0;

    switch (type)
    {
    case AIR:
        color = sf::Color::Transparent;
        break;
    case SOLID:
        color = sf::Color(100, 100, 100);
        break;
    case WATER:
        color = sf::Color::Blue;
        break;
    case SAND:
        color = sf::Color::Yellow;
        color.r += offset % 2;
        color.g += (offset + 5) % 2;
        color.b += (offset + 12) % 2;
        break;
    default:
        color = sf::Color::Magenta;
        break;
    }

    offset++;
}

Cell::~Cell()
{

}

void Cell::Update(int x, int y, unsigned char cycle, Chunk* c)
{
    CaveSand* inst = GetSand();

    //pretty sure this worries about if we move into a new chunk and we need to update the new shit
    if(this->cycle == cycle)
    {
        inst->Touch(x, y);
        return;
    }

    this->cycle = cycle;

    switch(type)
    {
        case AIR:
            break;
        case SAND:
            SandUpdate(x, y, c);
            break;
    }
}

CaveSand* Cell::GetSand()
{
    static CaveSand* sand = CaveSand::GetInstance();

    return sand;
}

Chunk* Cell::ChunkSteppy(int& x, int& y, Chunk* c)
{
    CaveSand* inst = GetSand();

    Chunk* cd = c;

    //check if we exceed bounds, then move the chunk and the position
    if(x > CHUNK_SIZE - 1) //MORE RIGHT
    {
        //change which chunk we're looking at
        cd = inst->GetChunk(c->xChunk + 1, c->yChunk);
        x %= CHUNK_SIZE; //clamp to funny
    }
    else if(x < 0) //MORE LEFT
    {
        //change which chunk we're looking at
        cd = inst->GetChunk(c->xChunk - 1, c->yChunk);
        x = CHUNK_SIZE - 1 + x; //shift that shit
    }
    
    //check again for y position
    if(y > CHUNK_SIZE - 1) //MORE DOWN
    {
        //change which chunk we're looking at
        cd = inst->GetChunk(c->xChunk, c->yChunk + 1);
        y %= CHUNK_SIZE; //funny clamp
    }
    else if(y < 0) //MORE UP
    {
        //change which chunk we're looking at
        cd = inst->GetChunk(c->xChunk, c->yChunk - 1);
        y = CHUNK_SIZE - 1 + y; //shift that shit

        //cycle--; //we do a little trolling
    }

    return cd;
}

bool Cell::Move(int& x, int& y, int xm, int ym, Chunk* c, bool replace)
{
    CaveSand* inst = GetSand();

    //ASSUMPTIONS:
    // 1. xm and ym are less than CHUNK_SIZE

    //next x and y pos
    int nx = x + xm;
    int ny = y + ym;
    
    Chunk* cd = c;

    //check if we exceed bounds, then move the chunk and the position
    if(nx > CHUNK_SIZE - 1) //MORE RIGHT
    {
        //change which chunk we're looking at
        cd = inst->GetChunk(c->xChunk + 1, c->yChunk);
        nx %= CHUNK_SIZE; //clamp to funny
    }
    else if(nx < 0) //MORE LEFT
    {
        //change which chunk we're looking at
        cd = inst->GetChunk(c->xChunk - 1, c->yChunk);
        nx = CHUNK_SIZE - 1 + nx; //shift that shit
    }
    
    //check again for y position
    if(ny > CHUNK_SIZE - 1) //MORE DOWN
    {
        //change which chunk we're looking at
        cd = inst->GetChunk(c->xChunk, c->yChunk + 1);
        ny %= CHUNK_SIZE; //funny clamp
    }
    else if(ny < 0) //MORE UP
    {
        //change which chunk we're looking at
        cd = inst->GetChunk(c->xChunk, c->yChunk - 1);
        ny = CHUNK_SIZE - 1 + ny; //shift that shit

        //cycle--; //we do a little trolling
    }

    //check if destination chunk is null
    if(!cd)
        return false;

    //AT THIS POINT, "cd" SHOULD BE A VALID DESTINATION CHUNK THAT WE'RE MOVING INTO AND "nx" AND "ny" SHOULD BE WITHIN BOUNDS

    //can we move there? (are we replacing or is the destination air?)
    if(replace || cd->cells[nx][ny].isAir())
    {
        cd->cells[nx][ny] = c->cells[x][y]; //move cell to destination
        c->cells[x][y] = Cell(AIR); //replace old position with air

        //touch!
        inst->Touch(nx, ny, cd);
        inst->Touch(x, y, c);

        //alter position values
        x = nx;
        y = ny;

        return true;
    }

    return false;
}

bool Cell::isAir()
{
    switch(type)
    {
        case AIR:
            return true;
        default:
            return false;
    }
}

bool Cell::isSolid()
{
    switch(type)
    {
        case SOLID:
            return true;
            break;
        case SAND:
            return true;
        default:
            return false;
    }
}

bool Cell::isLiquid()
{
    switch(type)
    {
        case WATER:
            return true;
        default:
            return false;
    }
}

bool Cell::isGas()
{
    switch(type)
    {
        case AIR:
            return true;
        default:
            return false;
    }
}

char Cell::PixelRand()
{
    static char num = 0;

    num++;

    return num;
}