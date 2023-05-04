#include "CaveSand.h"

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

void CaveSand::UpdateAll()
{
    cycle++;
    for(int i = 0; i < chunks.size(); i++)
    {
        chunks[i]->ChunkUpdate(cycle);
    }
}

void CaveSand::DrawAll(sf::RenderWindow* target)
{
    for (int i = 0; i < chunks.size(); i++)
    {
        chunks[i]->ChunkDraw(target);
    }
}

void CaveSand::UpdateChecker()
{
    cycle++;
    for(int i = 0; i < chunks.size(); i++)
    {
        if((chunks[i]->yChunk + chunks[i]->xChunk) % 2 == 0)
        {
            chunks[i]->ChunkUpdate(cycle);
        }
    }

    for(int i = 0; i < chunks.size(); i++)
    {
        if((chunks[i]->yChunk + chunks[i]->xChunk) % 2 == 1)
        {
            chunks[i]->ChunkUpdate(cycle);
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
            Set(toX, toY, from->cells[fromX % CHUNK_SIZE][fromY % CHUNK_SIZE], c);
            Set(fromX, fromY, Cell(AIR), from);
            
            //from->Touch(fromX % CHUNK_SIZE, fromY % CHUNK_SIZE);
        }
        else
        {
            //SWAPITY SWAP SWAP
            //std::swap(c->cells[fromX % CHUNK_SIZE][fromY % CHUNK_SIZE], c->cells[toX % CHUNK_SIZE][toY % CHUNK_SIZE]);
            //c->Touch(fromX % CHUNK_SIZE, fromY % CHUNK_SIZE);

            Set(toX, toY, c->cells[fromX % CHUNK_SIZE][fromY % CHUNK_SIZE], c);
            Set(fromX, fromY, Cell(AIR), c);
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

    if(chunk != nullptr)
    {
        chunk->cells[x % CHUNK_SIZE][y % CHUNK_SIZE] = p;
        chunk->Touch(x % CHUNK_SIZE, y % CHUNK_SIZE);
    }
}

void CaveSand::Set(int x, int y, Cell p, Chunk* c)
{
    if(c != nullptr)
    {
        c->cells[x % CHUNK_SIZE][y % CHUNK_SIZE] = p;
        c->Touch(x % CHUNK_SIZE, y % CHUNK_SIZE);
    }
}