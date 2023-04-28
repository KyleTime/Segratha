#include "../ChunkManager.h"

void Cell::SandUpdate(int xW, int yW)
{
    static ChunkManager* inst = ChunkManager::GetInstance();

    if( !inst->Move(xW, yW, xW, yW + 1))
    {
        if(!inst->Move(xW, yW, xW + 1, yW + 1))
        {
            if(!inst->Move(xW, yW, xW - 1, yW + 1)) {}
        }
    }
}