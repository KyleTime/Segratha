#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "Chunk.h"
#include "Camera.h"
#include <utility>
#include <vector>

class ChunkManager
{
    private:
        std::vector<Chunk*> chunks; //Chunks will live on the heap lol
        
        static ChunkManager* inst;
        
        unsigned char cycle; //current update cycle the game is on
        
        /// @brief unloads any chunks that are too far from the player
        void UnloadChunks();
        /// @brief loads in any needed chunks that the player is close to
        void LoadChunks();

    public:

        /// @brief loads a particular chunk position
        /// @param x x location of chunk
        /// @param y y location of chunk
        bool LoadAt(int x, int y);

        /// @brief loads a particular chunk position
        /// @param x x location of chunk
        /// @param y y location of chunk
        bool UnLoadAt(int x, int y);

        ChunkManager();
        ~ChunkManager();
        static ChunkManager* GetInstance();
        void UpdateAll();
        void DrawAll(sf::RenderWindow* target); //draws each function TODO: make this only draw visible chunks

        void UpdateChecker();

        void UpdateThreaded();

        //given coordinates on the chunk grid, grab that chunk and return a pointer
        Chunk* GetChunk(int xChunk, int yChunk);

        //given Cell coordinates, return a Chunk*
        Chunk* GetChunkCell(int xCell, int yCell);

        //given Cell coordinates, return a Cell*
        Cell* GetCellAt(int x, int y);

        //given a screen position in Cells, calculate which Cell is located there
        sf::Vector2i ScreenToCell(sf::Vector2i screen);

        //takes in a Cell position and returns a Chunk position
        sf::Vector2i CellToChunkPos(int x, int y);

        //given two Cell objects, return whether they're in the same chunk
        bool SameChunk(sf::Vector2i c1, sf::Vector2i c2);

        //Cell MOVE FUNCTIONS
        
        //Returns true if the destination Cell is air and the move was successful;
        // (fromX, fromY) is the coord of the Cell to move
        // (toX, toY) is the coord of the Cell to move to
        // KEEP IN MIND: These coords are in WORLD position, not raw chunk position
        bool Move(int fromX, int fromY, int toX, int toY);
        
        //Simply sets the position to the desired Cell
        //(x, y) is in world coordinates
        void Set(int x, int y, Cell p);

        //Simply sets the position to the desired Cell in the desired Chunk
        //(x, y) is in chunk coordinates
        void Set(int x, int y, Cell p, Chunk* c);
};

#endif