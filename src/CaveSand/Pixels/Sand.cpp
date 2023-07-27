#include "../CaveSand.h"

#include <iostream>

using namespace Segratha;

void Cell::SandUpdate(int x, int y, Chunk* c)
{
    static CaveSand* inst = CaveSand::GetInstance();

    //Chunk* c = inst->GetChunkCell(xW, yW);

    //int ydist = 1; //the distance we're gonna move (on the y)

    Move(x, y, 0, -1, c);
}