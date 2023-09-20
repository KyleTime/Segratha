#include "CaveSave.h"

using namespace Segratha;

CaveSave::CaveSave()
{

}

CaveSave::~CaveSave()
{

}

bool CaveSave::ChunkExists(int x, int y)
{
    //if we check the index of the chunk we're trying to find, do we find ourselves at the end of the thing?
    return (chunkIndex.find(std::make_pair(x, y)) != chunkIndex.end());
}

void CaveSave::SerializeChunk(Chunk c, std::fstream& file)
{

}

Chunk* DeserializeChunk(std::fstream file)
{
    return nullptr;
}

void CaveSave::WriteChunk(Chunk* c)
{
    //create filestream object
    std::fstream file;

    //open the file within our saves folder with the current selected name
    //ios::app -> open the file in append mode
    //ios::out -> enable writing to file
    //ios::in -> enable reading from file
    file.open(filePath + fileName, std::ios::app | std::ios::out | std::ios::in);
    
    if(ChunkExists(c->xChunk, c->yChunk))
    {
        //overwrite current chunk info

        //grab position in file from chunkIndex
        std::streampos pos = chunkIndex[std::make_pair(c->xChunk, c->yChunk)];
        //seek to position in file
        file.seekg(pos);

        //this should hopefully overwrite the data
        SerializeChunk(*c, file);
    }
    else
    {
        //write chunk to end of file

        //go to the end
        file.seekg(std::ios::end);

        //store the file position of the chunk
        chunkIndex[std::make_pair(c->xChunk, c->yChunk)] = file.tellp();
        
        SerializeChunk(*c, file);
    }

    //close once done
    file.close();
}

Chunk* CaveSave::LoadChunk(int x, int y)
{
    std::fstream file;

    //open the file within our saves folder with the current selected name
    //ios::app -> open the file in append mode
    //ios::out -> enable writing to file
    //ios::in -> enable reading from file
    file.open(filePath + fileName, std::ios::app | std::ios::out | std::ios::in);

    //grab position in file from chunkIndex
    std::streampos pos = chunkIndex[std::make_pair(x, y)];
    //seek to position in file
    file.seekg(pos);
    //deserialize chunk
    Chunk* loadedChunk = DeserializeChunk(file);

    file.close();

    return loadedChunk;

}