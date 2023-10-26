#include "../CaveSand.h"

using namespace Segratha;

void Cell::SandUpdate(int x, int y, Chunk* c)
{
    char curNum = PixelRand();

    if(!Move(x, y, 0, 1, c, inst) && curNum % 2 == 0)
        if(!Move(x, y, 1, 1, c, inst))
            Move(x, y, -1, 1, c, inst);
}