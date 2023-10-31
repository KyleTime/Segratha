#include "../CaveSand.h"

using namespace Segratha;

void Cell::SandUpdate(int x, int y, Chunk* c)
{
    CaveSand* inst = GetSand();

    char curNum = PixelRand();

    if(!Move(x, y, 0, 1, c, true, false) && curNum % 2 == 0)
        if(curNum % 3 == 0 || !Move(x, y, 1, 1, c, true, false))
            Move(x, y, -1, 1, c, true, false);
}