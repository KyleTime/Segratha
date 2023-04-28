#include "ChunkManager.h"

ChunkManager* ChunkManager::inst = nullptr;

ChunkManager::ChunkManager()
    : cycle(0)
{
}

ChunkManager::~ChunkManager()
{

}

ChunkManager* ChunkManager::GetInstance()
{
    if(inst == nullptr)
    {
        inst = new ChunkManager();
    }

    return inst;
}

void ChunkManager::UpdateAll()
{
    cycle++;
    for(int i = 0; i < chunks.size(); i++)
    {
        chunks[i]->ChunkUpdate(cycle);
    }
}

void ChunkManager::DrawAll(sf::RenderWindow* target)
{
    for (int i = 0; i < chunks.size(); i++)
    {
        chunks[i]->ChunkDraw(target);
    }
}

void ChunkManager::UpdateChecker()
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

void ChunkManager::UpdateThreaded()
{
    //first pass is  (x % 2 == 0 && y % 2 == 0)
    //second pass is (x % 2 == 1 && y % 2 == 0)
    //third pass is  (x % 2 == 0 && y % 2 == 1)
    //fourth pass is (x % 2 == 1 && y % 2 == 1)

    
}

bool ChunkManager::LoadAt(int x, int y)
{
    chunks.push_back(new Chunk(x, y));
    return true;
}

bool ChunkManager::UnLoadAt(int x, int y)
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

Chunk* ChunkManager::GetChunk(int xChunk, int yChunk)
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

Chunk* ChunkManager::GetChunkCell(int xCell, int yCell)
{
    if(xCell < 0 || yCell < 0)
        return nullptr;

    int xChunk = xCell / CHUNK_SIZE;
    int yChunk = yCell / CHUNK_SIZE;

    Chunk* chunk = GetChunk(xChunk, yChunk);

    return chunk;
}

Cell* ChunkManager::GetCellAt(int x, int y)
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

sf::Vector2i ChunkManager::ScreenToCell(sf::Vector2i pos)
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

sf::Vector2i ChunkManager::CellToChunkPos(int x, int y)
{
    return sf::Vector2i(x / CHUNK_SIZE, y / CHUNK_SIZE);
}

bool ChunkManager::SameChunk(sf::Vector2i c1, sf::Vector2i c2)
{
    return CellToChunkPos(c1.x, c1.y) == CellToChunkPos(c2.x, c2.y);
}

//Cell MANAGEMENT

bool ChunkManager::Move(int fromX, int fromY, int toX, int toY)
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

void ChunkManager::Set(int x, int y, Cell p)
{
    Chunk* chunk = GetChunkCell(x, y);

    if(chunk != nullptr)
    {
        chunk->cells[x % CHUNK_SIZE][y % CHUNK_SIZE] = p;
        chunk->Touch(x % CHUNK_SIZE, y % CHUNK_SIZE);
    }
}

void ChunkManager::Set(int x, int y, Cell p, Chunk* c)
{
    if(c != nullptr)
    {
        c->cells[x % CHUNK_SIZE][y % CHUNK_SIZE] = p;
        c->Touch(x % CHUNK_SIZE, y % CHUNK_SIZE);
    }
}