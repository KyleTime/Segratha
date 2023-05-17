#include "CaveSand.h"
#define RENDCHUNK_SIZE (CHUNK_SIZE / REND_FACTOR)

using namespace Segratha;

CaveSand* CaveSand::inst = nullptr;

CaveSand::CaveSand()
    : cycle(0)
{
}  

CaveSand::~CaveSand()
{

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

    //first, update all cells
    if(timer > updateTime)
    {
        UpdateThreaded();
        timer = 0;
    }
    timer += KyleTime::DeltaTime();

    //then, allocate render chunks
    AllocRenderers(target);

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

void CaveSand::AllocRenderers(sf::RenderWindow* target)
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

    lastCamPos = camPos; //make sure we store this shit

    //----------------------------------------

    //rectangle formula:
    //rect1, rect2
    //if(abs(rect1.x - rect2.x) < rect1.xSize/2 + rect2.xSize/2 && abs(rect1.y - rect2.y) < rect1.ySize/2 + rect2.ySize/2)

    threshold = sf::Vector2f(999999, 999999);

    float chunkSize = RENDCHUNK_SIZE * CELL_SIZE;

    int chunkRendPos = 0; //keep track of the chunk renderer we're looking at

    for(int i = 0; i < numRend; i++)
        rend[i].Unbind();

    for(int i = 0; i < chunks.size(); i++)
    {
        Chunk* c = chunks[i];

        if(!c)
            break;

        //calculate pixel position of the top left corner of the chunk
        sf::Vector2f chunkPos = sf::Vector2f((c->xChunk) * CHUNK_SIZE * CELL_SIZE, (c->yChunk) * CHUNK_SIZE * CELL_SIZE);
        
        //look at every subdivision of the chunk (by render)
        for(int x = 0; x < REND_FACTOR/2; x++)
        {
            for(int y = 0; y < REND_FACTOR/2; y++)
            {
                //calculate position of the render chunk
                sf::Vector2f rendChunkPos = chunkPos + sf::Vector2f(1.f/REND_FACTOR * CHUNK_SIZE * CELL_SIZE, 1.f/REND_FACTOR * CHUNK_SIZE * CELL_SIZE);

                if(abs(rendChunkPos.x - camPos.x) < chunkSize/2 + camSize.x/2 && abs(rendChunkPos.y - camPos.y) < chunkSize/2 + camSize.y)
                {
                    rend[chunkRendPos].Bind(c, rendChunkPos, x * RENDCHUNK_SIZE + y * RENDCHUNK_SIZE * RENDCHUNK_SIZE);
                    chunkRendPos++;

                    //do funny math to figure out the threshold
                    float xCamR = camPos.x + camSize.x/2; //calculate outer right edge of camera
                    float xCamL = camPos.x - camSize.x/2; //calculate outer left edge of camera

                    float xChunkR = rendChunkPos.x + chunkSize/2; //calculate outer right edge of chunk
                    float xChunkL = rendChunkPos.x - chunkSize/2; //calculate outer left edge of chunk

                    threshold.x = __min(__min(abs(xChunkR - xCamR), abs(xChunkL - xCamL)), threshold.x); //figure out which distance is smaller, then compare to current threshold

                    float yCamU = camPos.y + camSize.y/2; //calculate outer right edge of camera
                    float yCamD = camPos.y - camSize.y/2; //calculate outer left edge of camera

                    float yChunkU = rendChunkPos.y + chunkSize/2; //calculate outer right edge of chunk
                    float yChunkD = rendChunkPos.y - chunkSize/2; //calculate outer left edge of chunk

                    threshold.y = __min(__min(abs(yChunkU - yCamU), abs(yChunkD - yCamD)), threshold.y); //figure out which distance is smaller, then compare to current threshold
                
                    if(chunkRendPos == numRend)
                        return;
                }
            }
        }
    }
}

void CaveSand::ThreadHelper(int xMod, int yMod, std::vector<std::thread>& threads)
{
    //I have no idea why this is necessary, but it is
    unsigned char cycle = this->cycle;

    //loop for each chunk in chunks
    for (auto& chunk : chunks) {

        //test whether "chunk" part of the first pass
        if(chunk->xChunk % 2 == xMod && chunk->yChunk % 2 == yMod)
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

void CaveSand::Autoload(sf::Vector2f pos)
{
    float screenX = CAMERA::GetScreenWidth();
    float screenY = CAMERA::GetScreenHeight();
}

bool CaveSand::LoadAt(int x, int y)
{
    chunks.push_back(new Chunk(x, y));
    return true;
}

bool CaveSand::UnLoadAt(int x, int y)
{
    if(chunks.size() < 1)
        return false;
    
    for(auto i = chunks.begin(); i != chunks.end(); i++)
    {
        Chunk* c = *i;
        if(c->xChunk == x && c->yChunk == y)
        {
            chunks.erase(i);
            return true;
        }
    }

    return false;
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
    if(xCell < 0 || yCell < 0)
        return nullptr;

    int xChunk = xCell / CHUNK_SIZE;
    int yChunk = yCell / CHUNK_SIZE;

    Chunk* chunk = GetChunk(xChunk, yChunk);

    return chunk;
}

Cell* CaveSand::GetCellAt(int x, int y)
{
    Chunk* chunk = GetChunkCell(x, y);

    if(chunk != nullptr)
    {
        return &chunk->cells[x % CHUNK_SIZE][y % CHUNK_SIZE];
    }
    else
    {
        return nullptr;
    }

}

sf::Vector2i CaveSand::ScreenToCell(sf::Vector2i pos)
{
    sf::Vector2i fin = pos;

    //center pos
    fin.x += CAMERA::view.getCenter().x / CAMERA::zoom;
    fin.y += CAMERA::view.getCenter().y / CAMERA::zoom;

    //offset to center
    fin.x -= CAMERA::ASPECT_RATIO * CAMERA::SCREEN_SIZE * 1/2;
    fin.y -= CAMERA::SCREEN_SIZE * 1/2;

    //round to cell
    fin.x /= CELL_SIZE / CAMERA::zoom;
    fin.y /= CELL_SIZE / CAMERA::zoom;

    return fin;
}

sf::Vector2i CaveSand::CellToChunkPos(int x, int y)
{
    return sf::Vector2i(x / CHUNK_SIZE, y / CHUNK_SIZE);
}

bool CaveSand::SameChunk(sf::Vector2i c1, sf::Vector2i c2)
{
    return CellToChunkPos(c1.x, c1.y) == CellToChunkPos(c2.x, c2.y);
}

//Cell MANAGEMENT

bool CaveSand::Move(int fromX, int fromY, int toX, int toY)
{
    //figure out if we in the same chunk
    bool same = SameChunk(sf::Vector2i(fromX, fromY), sf::Vector2i(toX, toY));

    //grab chunk of "to"
    Chunk* c = GetChunkCell(toX, toY);

    if(c != nullptr && c->cells[toX % CHUNK_SIZE][toY % CHUNK_SIZE].isAir()) //if air, then move
    {
        //decide whether we need to grab another chunk
        if(!same)
        {
            Chunk* from = GetChunkCell(fromX, fromY);
            Set(toX % CHUNK_SIZE, toY % CHUNK_SIZE, from->cells[fromX % CHUNK_SIZE][fromY % CHUNK_SIZE], c);
            Set(fromX % CHUNK_SIZE, fromY % CHUNK_SIZE, Cell(AIR), from);
            
            //from->Touch(fromX % CHUNK_SIZE, fromY % CHUNK_SIZE);
        }
        else
        {
            //SWAPITY SWAP SWAP
            //std::swap(c->cells[fromX % CHUNK_SIZE][fromY % CHUNK_SIZE], c->cells[toX % CHUNK_SIZE][toY % CHUNK_SIZE]);
            //c->Touch(fromX % CHUNK_SIZE, fromY % CHUNK_SIZE);

            Set(toX % CHUNK_SIZE, toY % CHUNK_SIZE, c->cells[fromX % CHUNK_SIZE][fromY % CHUNK_SIZE], c);
            Set(fromX % CHUNK_SIZE, fromY % CHUNK_SIZE, Cell(AIR), c);
        }

        //touch at "to" position (it all we care about)
        //c->Touch(toX, toY);
    }
    else //if not air, stop
    {
        return false;
    }

    return true;
}

void CaveSand::Set(int x, int y, Cell p)
{
    Chunk* chunk = GetChunkCell(x, y);

    Set(x % CHUNK_SIZE, y % CHUNK_SIZE, p, chunk);
}

void CaveSand::Set(int x, int y, Cell p, Chunk* chunk)
{
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