#include "../CaveSand.h"

#include <iostream>

using namespace Segratha;

void Cell::SandUpdate(int xW, int yW)
{
    static CaveSand* inst = CaveSand::GetInstance();

    if( !inst->Move(xW, yW, xW, yW + 1))
    {
        if(!inst->Move(xW, yW, xW + 1, yW + 1))
        {
            if(!inst->Move(xW, yW, xW - 1, yW + 1)) {}
        }
    }
}