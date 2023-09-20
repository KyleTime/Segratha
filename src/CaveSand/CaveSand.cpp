#include "CaveSand.h"

using namespace Segratha;

CaveSand* CaveSand::inst = nullptr;

CaveSand::CaveSand()
    : cycle(0)
{
}  

CaveSand::~CaveSand()
{
    //TODO: UNLOAD ALL CHUNKS HERE!
}

CaveSand* CaveSand::GetInstance()
{
    if(inst == nullptr)
    {
        inst = new CaveSand();
    }

    return inst;
}

void CaveSand::FullRun(sf::RenderWindow* target, float updateTime)
{
    static float timer = 0;

    Autoload(target);

    //first, update all cells
    if(timer > updateTime)
    {
        UpdateThreaded();
        timer = 0;
    }
    timer += KyleTime::DeltaTime();

    //then, allocate render chunks
    RenderGroup(target);

    //Finally, draw all render chunks
    DrawAll(target);
}

void CaveSand::DrawAll(sf::RenderWindow* target)
{
    //draw out all current render chunks
    for (int i = 0; i < numRend; i++)
    {
        rend[i].ChunkDraw(target);
    }
}

int CaveSand::GetNumberOfChunks()
{
    return chunks.size();
}

void CaveSand::RenderGroup(sf::RenderWindow* target)
{
    //grab view
    sf::View view = target->getView();

    sf::Vector2f camPos = view.getCenter(); //grab the camera position
    sf::Vector2f camSize = view.getSize(); //grab camera size

    //DO I HAVE TO?---------------------------

    if(abs(camPos.x - lastCamPos.x) < threshold.x && abs(camPos.y - lastCamPos.y) < threshold.y)
    {
        //NO
        return;
    }

    //----------------------------------------

    //rectangle formula:
    //rect1, rect2
    //if(abs(rect1.x - rect2.x) < rect1.xSize/2 + rect2.xSize/2 && abs(rect1.y - rect2.y) < rect1.ySize/2 + rect2.ySize/2)

    threshold = sf::Vector2f(999999, 999999);

    float chunkSize = REND_SIZE * CELL_SIZE;

    int chunkRendPos = 0; //keep track of the chunk renderer we're looking at

    sf::Vector2i min = (sf::Vector2i((camPos.x - camSize.x/2)/CELL_SIZE, (camPos.y - camSize.y/2)/CELL_SIZE)); //calculate where cell at top left of camera is located
    sf::Vector2i max = (sf::Vector2i((camPos.x + camSize.x/2)/CELL_SIZE, (camPos.y + camSize.y/2)/CELL_SIZE)); //calculate where cell at bottom right of camera is located

    sf::Vector2i minPos = sf::Vector2i(min.x/(REND_SIZE) - 1, min.y/(REND_SIZE) - 1); //get chunk pos of min
    sf::Vector2i maxPos = sf::Vector2i(max.x/(REND_SIZE), max.y/(REND_SIZE)); //get chunk pos of max

    for(int x = minPos.x; x <= maxPos.x; x++)
    {
        for(int y = minPos.y; y <= maxPos.y; y++)
        {
            int xChunk = x*REND_SIZE / CHUNK_SIZE;
            int yChunk = y*REND_SIZE / CHUNK_SIZE;

            if(x*REND_SIZE < 0 && x*REND_SIZE % CHUNK_SIZE != 0)
                xChunk -= 1;
            if(y*REND_SIZE < 0 && y*REND_SIZE % CHUNK_SIZE != 0)
                yChunk -= 1;

            Chunk* c = GetChunk(xChunk, yChunk);

            if(c == nullptr)
                continue;

            rend[chunkRendPos].Bind(x, y, c); //Bind the render chunk
            chunkRendPos++; //increment render counter

            sf::Vector2f chunkPos = sf::Vector2f((x + 0.5f) * chunkSize, (y + 0.5f) * chunkSize);

            //do funny math to figure out the threshold
            float xCamR = camPos.x + camSize.x/2; //calculate outer right edge of camera
            float xCamL = camPos.x - camSize.x/2; //calculate outer left edge of camera

            float xChunkR = chunkPos.x + chunkSize/2; //calculate outer right edge of chunk
            float xChunkL = chunkPos.x - chunkSize/2; //calculate outer left edge of chunk

            threshold.x = __min(__min(abs(xChunkR - xCamR), abs(xChunkL - xCamL)), threshold.x); //figure out which distance is smaller, then compare to current threshold

            float yCamU = camPos.y + camSize.y/2; //calculate outer right edge of camera
            float yCamD = camPos.y - camSize.y/2; //calculate outer left edge of camera

            float yChunkU = chunkPos.y + chunkSize/2; //calculate outer right edge of chunk
            float yChunkD = chunkPos.y - chunkSize/2; //calculate outer left edge of chunk

            threshold.y = __min(__min(abs(yChunkU - yCamU), abs(yChunkD - yCamD)), threshold.y); //figure out which distance is smaller, then compare to current threshold

            //we out of renderers
            if(chunkRendPos >= numRend)
                break;
        }

        //we out of renderers
        if(chunkRendPos >= numRend)
            break;
    }
 
    lastCamPos = camPos; //make sure we store this shit
}

void CaveSand::ThreadHelper(int xMod, int yMod, std::vector<std::thread>& threads)
{
    //I have no idea why this is necessary, but it is
    unsigned char cycle = this->cycle;

    //loop for each chunk in chunks
    for (auto& chunk : chunks) {

        //test whether "chunk" part of the first pass
        if(abs(chunk->xChunk % 2) == xMod && abs(chunk->yChunk % 2) == yMod)
        {
            //create a lambda that takes in the chunk and the cycle, then updates the chunk, and add it to threads
            threads.emplace_back([&chunk, &cycle]() {
                chunk->ChunkUpdate(cycle);
            });
        }
    }

    //wait for each thread to finish
    for (auto& thread : threads) {
        thread.join();
    }

    //clear all threads
    threads.clear();
}

void CaveSand::UpdateThreaded()
{
    //first pass is  (x % 2 == 0 && y % 2 == 0)
    //second pass is (x % 2 == 1 && y % 2 == 0)
    //third pass is  (x % 2 == 0 && y % 2 == 1)
    //fourth pass is (x % 2 == 1 && y % 2 == 1)

    cycle++;

    //there's probably a better way to do this, but "int cycle = cycle" just works
    unsigned char cycle = this->cycle;

    //list of current threads
    std::vector<std::thread> threads;

    //PASS 1 ----------------------------------------
    ThreadHelper(0, 0, threads);
    //-----------------------------------------------

    //PASS 2 ----------------------------------------
    ThreadHelper(1, 0, threads);
    //-----------------------------------------------

    //PASS 3 ----------------------------------------
    ThreadHelper(0, 1, threads);
    //-----------------------------------------------

    //PASS 4 ----------------------------------------
    ThreadHelper(1, 1, threads);
    //-----------------------------------------------
}

void CaveSand::Autoload(sf::RenderWindow* target)
{
    //so basically here's how this is going to go
    //
    //   we're going to say that whenever the player gets close enough to a new chunk, we load new chunks and unload the furthest away
    // to do that, we're going to need the player chunk pos

    static sf::Vector2i playerCell = sf::Vector2i(99999, 99999);

    //player cell pos right now
    sf::Vector2i cellPos = WorldToCell(target->getView().getCenter());

    //if the partial chunk pos hasn't changed, we don't need to worry
    if(playerCell / (CHUNK_SIZE / 4) == cellPos / (CHUNK_SIZE / 4))
        return;

    playerCell = cellPos;

    sf::Vector2i chunkPos = CellToChunkPos(cellPos.x, cellPos.y);

    //LOAD NEW CHUNKS----------------------------------------
    sf::Vector2i relPos = sf::Vector2i();

    //left side
    if(abs(cellPos.x) % CHUNK_SIZE < CHUNK_SIZE / 2)
    {
        relPos.x = -1;
    }
    //right side
    else
    {
        relPos.x = 1;
    }

    //top side
    if(abs(cellPos.y) % CHUNK_SIZE < CHUNK_SIZE / 2)
    {
        relPos.y = -1;
    }
    //bottom side
    else
    {
        relPos.y = 1;
    }

    if(cellPos.x < 0)
        relPos.x *= -1;
    if(cellPos.y < 0)
        relPos.y *= -1;

    /*static int j = 0;
    std::cout << "iteration: " << j << "-------------------------------" << std::endl;
    std::cout << chunkPos.x << ", " << chunkPos.y << std::endl;
    std::cout << "cellPos: " << cellPos.x << ", " << cellPos.y << std::endl;
    j++;*/

    //load chunk I'm in
    LoadAt(chunkPos.x, chunkPos.y);

    //load new chunks
    LoadAt(chunkPos.x + relPos.x, chunkPos.y + relPos.y);
    LoadAt(chunkPos.x + relPos.x, chunkPos.y);
    LoadAt(chunkPos.x, chunkPos.y + relPos.y);

    //check number of chunks
    int size = chunks.size();

    //eliminate chunks that are farther than 1 unit away from the current chunk until there are only 12
    for(auto i = chunks.begin(); i != chunks.end() && chunks.size() > 12; i++)
    {
        Chunk* c = *i;
        if(abs(c->xChunk - chunkPos.x) >= 2 || abs(c->yChunk - chunkPos.y) >= 2)
        {
            UnLoad(c);
            chunks.erase(i);
            //i--;
        }
    }
}

bool CaveSand::LoadAt(int x, int y)
{
    //std::cout << "Loaded at: " << x << ", " << y << std::endl;

    //check if the chunk already exists
    if(GetChunk(x, y))
    {
        FullTouch(x, y);
        return false;
    }

    Chunk* c = caveSave.LoadChunk(x, y);
    chunks.push_back(c);

    return true;
}

bool CaveSand::UnLoad(Chunk* c)
{
    caveSave.WriteChunk(c);
    delete c;

    return true;
}

// bool CaveSand::UnLoadAt(int x, int y)
// {
//     if(chunks.size() < 1)
//         return false;
    
//     for(auto i = chunks.begin(); i != chunks.end(); i++)
//     {
//         Chunk* c = *i;
//         if(c->xChunk == x && c->yChunk == y)
//         {
//             chunks.erase(i);
//             return true;
//         }
//     }

//     return false;
// }

Chunk* CaveSand::GetChunk(sf::Vector2i pos)
{
    return GetChunk(pos.x, pos.y);
}

Chunk* CaveSand::GetChunk(int xChunk, int yChunk)
{
    for(int i = 0; i < chunks.size(); i++)
    {
        if(chunks[i]->xChunk == xChunk && chunks[i]->yChunk == yChunk)
        {
            return chunks[i];
        }
    }

    return nullptr;
}

Chunk* CaveSand::GetChunkCell(int xCell, int yCell)
{
    //if(xCell < 0 || yCell < 0)
    //   return nullptr;

    //xCell -= 1;
    //yCell -= 1;

    Chunk* chunk = GetChunk(CellToChunkPos(xCell, yCell));

    return chunk;
}

Cell* CaveSand::GetCellAt(int x, int y)
{
    Chunk* chunk = GetChunkCell(x, y);
    sf::Vector2i rel = RelCellPos(x, y);
    if(chunk != nullptr)
    {
        return &chunk->cells[rel.x][rel.y];
    }
    else
    {
        return nullptr;
    }

}

sf::Vector2i CaveSand::ScreenToCell(sf::Vector2i pos)
{
    sf::Vector2i fin = pos;

    //offset to center
    fin.x -= CAMERA::ASPECT_RATIO * CAMERA::SCREEN_SIZE * 1/2;
    fin.y -= CAMERA::SCREEN_SIZE * 1/2;

    //convert to game size
    fin.x *= (float)CAMERA::GAME_SIZE / CAMERA::SCREEN_SIZE;
    fin.y *= (float)CAMERA::GAME_SIZE / CAMERA::SCREEN_SIZE;


    //center pos
    fin.x += CAMERA::view.getCenter().x / CAMERA::zoom;
    fin.y += CAMERA::view.getCenter().y / CAMERA::zoom;

    //round to cell
    fin.x /= CELL_SIZE / CAMERA::zoom;
    fin.y /= CELL_SIZE / CAMERA::zoom;

    return fin;
}

sf::Vector2i CaveSand::WorldToCell(sf::Vector2f world)
{
    sf::Vector2f fin = world;

    //change units to cells
    fin /= CELL_SIZE;

    if(world.x < 0)
        fin.x--;
    if(world.y < 0)
        fin.y--;

    return (sf::Vector2i)fin;
}

sf::Vector2i CaveSand::CellToChunkPos(sf::Vector2i cellPos)
{
    return CellToChunkPos(cellPos.x, cellPos.y);
}

sf::Vector2i CaveSand::CellToChunkPos(int x, int y)
{
    if(x < -511)
        x++;
    if(y < -511)
        y++;

    int xChunk = x / CHUNK_SIZE;
    int yChunk = y / CHUNK_SIZE;

    if(x < 0)
        xChunk -= 1;
    if(y < 0)
        yChunk -= 1;
    
    return sf::Vector2i(xChunk, yChunk);
}

bool CaveSand::SameChunk(sf::Vector2i c1, sf::Vector2i c2)
{
    return CellToChunkPos(c1.x, c1.y) == CellToChunkPos(c2.x, c2.y);
}

sf::Vector2i CaveSand::RelCellPos(sf::Vector2i c)
{
    // if(c.x < 0)
    //     c.x += 1;
    
    //if(c.y < 0)
    //    c.y += 1;

    int x = ((c.x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int y = ((c.y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;

    //if(c.x == 0)
    //{
    //    std::cout << "X at 0 IS " << x << std::endl;
    //    x = 0;
    //} 

    return sf::Vector2i(x, y);
}

sf::Vector2i CaveSand::RelCellPos(int x, int y)
{
    return RelCellPos(sf::Vector2i(x, y));
}

void CaveSand::FullTouch(int x, int y)
{
    Chunk* c = GetChunk(x, y);

    Touch(0, 0, c);
    Touch(CHUNK_SIZE - 1, CHUNK_SIZE - 1, c);
}

//Cell MANAGEMENT

bool CaveSand::Move(int fromX, int fromY, int toX, int toY)
{
    //figure out if we in the same chunk
    bool same = SameChunk(sf::Vector2i(fromX, fromY), sf::Vector2i(toX, toY));

    //grab chunk of "to"
    Chunk* c = GetChunkCell(toX, toY);
    sf::Vector2i to = RelCellPos(toX, toY);

    sf::Vector2i fromP = RelCellPos(fromX, fromY);

    if(c != nullptr && c->cells[to.x][to.y].isAir()) //if air, then move
    {
        //decide whether we need to grab another chunk
        if(!same)
        {
            Chunk* from = GetChunkCell(fromX, fromY);

            if(!from)
            {
                return false;

                std::cout << "---------------------------------------------------------------------------------------------------------------" << std::endl;
                std::cout << "putting (" << fromX << ", " << fromY << ") into RelCel, got (";
                std::cout << fromP.x << ", " << fromP.y << ") " << std::endl;
                std::cout << "CelltoChunk is " << CellToChunkPos(fromX, fromY).x << ", " << CellToChunkPos(fromX, fromY).y << std::endl;


                std::cout << "LISTING CHUNKS>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
                for(int i = 0; i < chunks.size(); i++)
                {
                    std::cout << "chunk: " << chunks[i]->xChunk << ", " << chunks[i]->yChunk << std::endl;
                }

                std::cout << "---------------------------------------------------------------------------------------------------------------" << std::endl;
            }

            Set(to.x, to.y, from->cells[fromP.x][fromP.y], c);
            Set(fromP.x, fromP.y, Cell(AIR), from);
            
            //from->Touch(fromX % CHUNK_SIZE, fromY % CHUNK_SIZE);
        }
        else
        {
            //SWAPITY SWAP SWAP
            //std::swap(c->cells[fromX % CHUNK_SIZE][fromY % CHUNK_SIZE], c->cells[toX % CHUNK_SIZE][toY % CHUNK_SIZE]);
            //c->Touch(fromX % CHUNK_SIZE, fromY % CHUNK_SIZE);

            Set(to.x, to.y, c->cells[fromP.x][fromP.y], c);
            Set(fromP.x, fromP.y, Cell(AIR), c);
        }

        //touch at "to" position (it all we care about)
        //c->Touch(toX, toY);
    }
    else //if not air, stop
    {
        //if(c == nullptr)
            //std::cout << "got null chunk at cell position: " << toX << ", " << toY << std::endl;

        return false;
    }

    return true;
}

void CaveSand::Set(int x, int y, Cell p)
{
    Chunk* chunk = GetChunkCell(x, y);
    sf::Vector2i cc = RelCellPos(x, y);

    Set(cc.x, cc.y, p, chunk);
}

void CaveSand::Set(int x, int y, Cell p, Chunk* chunk)
{
    //std::cout << "POSITION OF SET: " << x << ", " << y << std::endl;

    if(chunk == nullptr)
        return;

    chunk->cells[x][y] = p;
    Touch(x, y, chunk);
}

void CaveSand::Touch(int x, int y)
{
    Chunk* chunk = GetChunkCell(x, y);

    Touch(x % CHUNK_SIZE, y % CHUNK_SIZE, chunk);
}

void CaveSand::Touch(int x, int y, Chunk* chunk)
{
    if(chunk == nullptr)
        return;

    chunk->Touch(x, y);

    if(x == 0)
    {
        Chunk* c = GetChunk(chunk->xChunk - 1, chunk->yChunk);
        if(c)
            c->Touch(CHUNK_SIZE - 1, y);
    }
    else if(x == CHUNK_SIZE - 1)
    {
        Chunk* c = GetChunk(chunk->xChunk + 1, chunk->yChunk);
        if(c)
            c->Touch(0, y);
    }

    if(y == 0)
    {
        Chunk* c = GetChunk(chunk->xChunk, chunk->yChunk - 1);
        if(c)
            c->Touch(x, CHUNK_SIZE - 1);
    }
    else if(y == CHUNK_SIZE - 1)
    {
        Chunk* c = GetChunk(chunk->xChunk, chunk->yChunk + 1);
        if(c)
            c->Touch(x, 0);
    }
}