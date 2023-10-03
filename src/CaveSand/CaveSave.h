#ifndef CAVESAVE_H
#define CAVESAVE_H

#include "Chunk.h"
#include <iostream>
#include <fstream>

namespace Segratha
{
    class CaveSave
    {
        private:
            std::string filePath = "/WorldSaves/";
            std::string fileName = "world.goober";

            std::map<std::pair<int, int>, std::streampos> chunkIndex;


            /// @brief At the current position of the filestream, write the contents of the chunk
            /// @param c The Chunk to serialize
            /// @param file The filestream to write to
            void SerializeChunk(Chunk* c, std::fstream& file);

            /// @brief At the current position of the filestream, load the chunk
            /// @param file The filestream to deserialize from
            Chunk* DeserializeChunk(int x, int y, std::fstream& file);

        public:
            /// @brief returns true only if the save file contains a chunk at the coordinates
            /// @param x X coordinate of chunk to locate
            /// @param y Y coordinate of chunk to locate
            /// @return True if the chunk exists
            bool ChunkExists(int x, int y);

            CaveSave();
            ~CaveSave();

            /// @brief Writes a chunk to the current world file
            /// @param c The chunk to write
            void WriteChunk(Chunk* c);

            /// @brief Loads a chunk from disk and puts it into the funny heap (make sure you keep a reference to it!)
            /// @param x X coordinate of the chunk to load
            /// @param y Y coordinate of the chunk to load
            /// @return A pointer to the loaded chunk
            Chunk* LoadChunk(int x, int y);
    };
}


#endif