#ifndef CHUNKHEADER_H
#define CHUNKHEADER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"

namespace Segratha
{
    const int CHUNK_SIZE = 512;

    class Chunk
    {
        private:
  
            //xChunk and yChunk refer to the grid of prospective chunks
            //[(0,0)][(1,0)][(2,0)] <- each of these are chunks
            //[(0,1)][(1,1)][(2,1)]
            //[(0,2)][(1,2)][(2,2)] #also it can go negative
            //
            //The Load() function grabs pixels from that position on disk
            void Load();
            //Returns the Chunk to the disk.
            void Unload();

        public:
            //whether this chunk is active
            sf::Vector2i rect1 = sf::Vector2i(0, 0); //update from here to rect2, bottom left
            sf::Vector2i rect2 = sf::Vector2i(63, 63); //update from rect1 to here, top right

            int xChunk; int yChunk;
            Cell cells[CHUNK_SIZE][CHUNK_SIZE];
            
            unsigned char updateCycles = 1;

            Chunk(int xChunk, int yChunk);
            ~Chunk();

            //updates every pixel in the chunk
            void ChunkUpdate(unsigned char cycle);

            //At (x, y), tell the chunk a pixel just updated there
            void Touch(int x, int y);
    };
}

#endif