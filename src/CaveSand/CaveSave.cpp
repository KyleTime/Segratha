#include "CaveSave.h"

using namespace Segratha;

CaveSave::CaveSave()
{
    //clear/create the thingy
    std::ofstream ofs;
    ofs.open("test.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

CaveSave::~CaveSave()
{

}

bool CaveSave::ChunkExists(int x, int y)
{
    //if we check the index of the chunk we're trying to find, do we find ourselves at the end of the thing?
    return !chunkIndex.empty() && (chunkIndex.find(std::make_pair(x, y)) != chunkIndex.end());
}

void CaveSave::SerializeChunk(Chunk* c, std::fstream& file)
{
    //loop through each cell and save it in the format <type><red><green><blue><alpha>
    for(int x = 0; x < CHUNK_SIZE; x++)
        for(int y = 0; y < CHUNK_SIZE; y++)
        {
            Cell cell = c->cells[x][y];

            char t = cell.type;
            char r = cell.color.r;
            char g = cell.color.g;
            char b = cell.color.b;
            char a = cell.color.a;

            file.write(&t, 1);
            file.write(&r, 1);
            file.write(&g, 1);
            file.write(&b, 1);
            file.write(&a, 1);
        }
}

Chunk* CaveSave::DeserializeChunk(int x, int y, std::fstream& file)
{
    //create an empty chunka
    Chunk* c = new Chunk(x, y);

    for(int x = 0; x < CHUNK_SIZE; x++)
    {
        for(int y = 0; y < CHUNK_SIZE; y++)
        {
            //read in the data
            char tipo, r, g, b, a;
            
            file.read(&tipo, 1);
            file.read(&r, 1);
            file.read(&g, 1);
            file.read(&b, 1);
            file.read(&a, 1);

            //std::cout << "Grabbed cell with ID: " << (int)tipo << std::endl;

            //create the funny cell to insert into chunk
            Cell funnyCell = Cell((cell_type)tipo);
            //funnyCell.color = sf::Color((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
            funnyCell.color = sf::Color(255, 0, 0, (unsigned char)a);

            c->cells[x][y] = funnyCell;
        }
    }

    return c;
}

void CaveSave::WriteChunk(Chunk* c)
{
    std::cout << "Beginning Chunk Write..." << std::endl;

    //create filestream object
    std::fstream file;

    //open the file within our saves folder with the current selected name
    //ios::out -> enable writing to file
    //ios::in -> enable reading from file
    file.open("test.goober", std::fstream::binary | std::fstream::in | std::fstream::out);

    if(!file)
    {
        std::cerr << "File failed to open (Write)" << std::endl;
        return;
    }

    if(ChunkExists(c->xChunk, c->yChunk))
    {
        std::cout << "Chunk Exists!" << std::endl;
        //overwrite current chunk info

        //seek to position in file
        file.seekg(chunkIndex[std::make_pair(c->xChunk, c->yChunk)]);

        //this should hopefully overwrite the data
        SerializeChunk(c, file);
    }
    else
    {
        //write chunk to end of file

        //go to the end
        file.seekg(0, std::ios::end);

        //store the file position of the chunk
        chunkIndex[std::make_pair(c->xChunk, c->yChunk)] = file.tellg();
        
        SerializeChunk(c, file);

        std::cout << "added chunk to end, number of chunks in file: " << chunkIndex.size() << std::endl;
    }

    //close once done
    file.close();
}

Chunk* CaveSave::LoadChunk(int x, int y)
{
    std::cout << "Loading Chunk at " << x << ", " << y << std::endl;

    if(ChunkExists(x, y))
    {
        std::fstream file;

        //open the file within our saves folder with the current selected name
        //ios::out -> enable writing to file
        //ios::in -> enable reading from file
        file.open("test.goober", std::fstream::binary | std::fstream::in | std::fstream::out);

        if(!file)
        {
            std::cerr << "File failed to open (Load)" << std::endl;
            return nullptr;
        }

        //grab position in file from chunkIndex
        std::streampos pos = chunkIndex[std::make_pair(x, y)];
        //seek to position in file
        file.seekg(pos);

        std::cout << "Deserializing Chunk..." << std::endl;

        //deserialize chunk
        Chunk* loadedChunk = DeserializeChunk(x, y, file);

        file.close();

        return loadedChunk;
    }
    else
    {
        //if we don't have a chunk in memory, just make a new one lol
        Chunk* fresh = new Chunk(x, y);
        return fresh;
    }
}