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
            
            //currently, the Load() method fills the chunk with air
            void Load();

        public:
            //whether this chunk is active
            sf::Vector2i rect1 = sf::Vector2i(0, 0); //update from here to rect2, bottom left
            sf::Vector2i rect2 = sf::Vector2i(CHUNK_SIZE - 1, CHUNK_SIZE - 1); //update from rect1 to here, top right

            int xChunk; int yChunk;
            Cell cells[CHUNK_SIZE][CHUNK_SIZE];
            
            unsigned char updateCycles = 1;

            Chunk(int xChunk, int yChunk);
            ~Chunk();

            //updates every cell in the chunk
            void ChunkUpdate(unsigned char cycle);

            //At (x, y), tell the chunk a cell just updated there
            void Touch(int x, int y);
            
            //wake all cell at the edges of the chunk
            void EdgeTouch(int x, int y);
    };
}

#endif