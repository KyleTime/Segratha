#include "../CaveSand.h"

using namespace Segratha;

void Cell::SandUpdate(int x, int y, Chunk* c)
{
    static CaveSand* inst = CaveSand::GetInstance();

    //Chunk* c = inst->GetChunkCell(xW, yW);

    //int ydist = 1; //the distance we're gonna move (on the y)

    if(!Move(x, y, 0, 1, c))
        if(!Move(x, y, 1 * (-1 * (cycle % 2)), 1, c))
            Move(x, y, -1 * (-1 * ((cycle + 1) % 2)), 1, c);

    
}