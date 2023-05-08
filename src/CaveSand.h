#ifndef CAVESAND_H
#define CAVESAND_H

#include "Chunk.h"
#include "Camera.h"
#include <utility>
#include <vector>
#include <thread>

namespace Segratha
{
    class CaveSand
    {
        private:
            std::vector<Chunk*> chunks; //Chunks will live on the heap lol
            
            static CaveSand* inst;
            
            unsigned char cycle; //current update cycle the game is on
            
            /// @brief unloads any chunks that are too far from the player
            void UnloadChunks();
            /// @brief loads in any needed chunks that the player is close to
            void LoadChunks();

            /// @brief Runs through the process of multithreading a set of chunks
            /// @param xMod (0 - 1) processes chunk if chunk->xChunk % 2 == xMod
            /// @param yMod (0 - 1) processes chunk if chunk->yChunk % 2 == yMod
            /// @param threads A reference to the current vector of threads
            void ThreadHelper(int xMod, int yMod, std::vector<std::thread>& threads);

        public:

            CaveSand();
            ~CaveSand();
            static CaveSand* GetInstance();

            /// @brief loads a particular chunk position
            /// @param x x location of chunk
            /// @param y y location of chunk
            bool LoadAt(int x, int y);

            /// @brief @brief loads a particular chunk position
            /// @param x x location of chunk
            /// @param y y location of chunk
            /// @return whether the operation was successful
            bool UnLoadAt(int x, int y);

            ///Updates every chunk in the order they were loaded
            void UpdateAll();

            /// @brief Draws EVERY chunk to the screen
            void DrawAll(sf::RenderWindow* target);

            ///@brief Updates in a Checkerboard pattern, works a bit better than UpdateAll(), if a bit slower
            void UpdateChecker();

            ///@brief Updates all chunks using multiple CPU threads
            void UpdateThreaded();

            /// @brief Based on the position given, it figures out what chunks must be loaded or unloaded.
            /// @param position position to focus on, generally the player.
            void Autoload(sf::Vector2f position);

            ///@brief given coordinates on the chunk grid, grab that chunk and return a pointer
            Chunk* GetChunk(int xChunk, int yChunk);

            ///@brief given Cell coordinates, return a Chunk*
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
            void Set(int x, int y, Cell p, Chunk* chunk);

            //touches a chunk at the given Cell coordinates
            void Touch(int x, int y);

            //touches a specified chunk at the given chunk coordinates
            void Touch(int x, int y, Chunk* chunk);
    };
}

#endif