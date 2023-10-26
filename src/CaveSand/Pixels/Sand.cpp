#include "../CaveSand.h"

using namespace Segratha;

void Cell::SandUpdate(int x, int y, Chunk* c)
{
    CaveSand* inst = GetSand();

    //Chunk* c = inst->GetChunkCell(xW, yW);

    //int ydist = 1; //the distance we're gonna move (on the y)

    char curNum = PixelRand();

    if(!Move(x, y, 0, 1, c) && curNum % 2 == 0)
        if(!Move(x, y, 1, 1, c))
            Move(x, y, -1, 1, c);
}