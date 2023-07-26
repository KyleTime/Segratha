#include "CaveSand.h"

using namespace Segratha;

Cell::Cell()
    : color(sf::Color::Magenta), cycle(0), type(AIR)
{

}

Cell::Cell(cell_type type)
    : cycle(0), type(type)
{
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
        break;
    default:
        color = sf::Color::Magenta;
        break;
    }
}

Cell::~Cell()
{

}

void Cell::Update(int x, int y, unsigned char cycle)
{
    static CaveSand* inst = CaveSand::GetInstance();

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
            SandUpdate(x, y);
            break;
    }
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
        case SOLID || SAND:
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