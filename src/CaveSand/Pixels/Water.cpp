#include "../CaveSand.h"

using namespace Segratha;

void Cell::WaterUpdate(int x, int y, int speed, Chunk* c)
{
    CaveSand* inst = GetSand();

    //move down

    char curNum = PixelRand();

    curNum = (curNum % 2) - ((curNum + 1) % 2); //returns either -1 or 1 I think

    if(!Move(x, y, 0, 1, c, true, false))
    {
        char sleep = PixelRand() % 5;

        if(sleep == 0)
        {
            return;
        }

        for(int i = 0; i < std::min(speed, 6); i++)
        {
            if(!Move(x, y, curNum, 0, c, true, false))
            {
                curNum *= -1;
                speed++;
            }

            Move(x,y,0,1,c, true, false);
        }
    }
    else if(PixelRand() % 5 == 0)
    {
        Move(x, y, curNum, 0, c, true, false);
    }
}