#ifndef CAVESAND_H
#define CAVESAND_H

#include "Chunk.h"
#include "ChunkRend.h"
#include "Camera.h"
#include "KyleTime.hpp"
#include <utility>
#include <vector>
#include <thread>

namespace Segratha
{
    class CaveSand
    {
        private:
            std::vector<Chunk*> chunks; //Chunks will live on the heap lol
            
            const static int numRend = 144; //determines how many renderers we got
            ChunkRend rend[numRend]; //renderers will live on the heap as well

            static CaveSand* inst; //static instance of this class
            
            sf::Vector2f lastCamPos = sf::Vector2f(-999, -999); //used to tell if the camera has moved for rendering purposes
            
            sf::Vector2f threshold; //if the camera has moved more than the threshold, we gotta realloc renderers

            unsigned char cycle; //current update cycle the game is on

            /// @brief Runs through the process of multithreading a set of chunks
            /// @param xMod (0 - 1) processes chunk if chunk->xChunk % 2 == xMod
            /// @param yMod (0 - 1) processes chunk if chunk->yChunk % 2 == yMod
            /// @param threads A reference to the current vector of threads
            void ThreadHelper(int xMod, int yMod, std::vector<std::thread>& threads);

            /// @brief Draws EVERY chunk to the screen
            void DrawAll(sf::RenderWindow* target);

            /// @brief puts each chunk renderer on its place in the screen, they'll figure out chunks later
            /// @param target the window to read camera data from
            void RenderGroup(sf::RenderWindow* target);

            ///@brief Updates all chunks using multiple CPU threads
            void UpdateThreaded();

            /// @brief Given a chunk position, fully update that chunk
            /// @param x chunk x position
            /// @param y chunk y position
            void FullTouch(int x, int y);
        public:

            CaveSand();
            ~CaveSand();
            static CaveSand* GetInstance();

            /// @brief Updates everything and renders
            /// @param target Window to render to
            void FullRun(sf::RenderWindow* target, float updateTime);

            /// @brief loads a particular chunk position
            /// @param x x location of chunk
            /// @param y y location of chunk
            bool LoadAt(int x, int y);

            /// @brief @brief loads a particular chunk position
            /// @param x x location of chunk
            /// @param y y location of chunk
            /// @return whether the operation was successful
            bool UnLoadAt(int x, int y);

            /// @brief Based on the position given, it figures out what chunks must be loaded or unloaded.
            /// @param position position to focus on, generally the player.
            void Autoload(sf::RenderWindow* target);

            ///@brief given coordinates on the chunk grid, grab that chunk and return a pointer
            Chunk* GetChunk(int xChunk, int yChunk);

            ///@brief given Cell coordinates, return a Chunk*
            Chunk* GetChunkCell(int xCell, int yCell);

            //given Cell coordinates, return a Cell*
            Cell* GetCellAt(int x, int y);

            //NOTE ABOUT WORLD AND SCREEN: 
            //screen coordinates begin at the top left of the screen at 0, 0 and go to the width and height of the window
            //world coordinates begin at a point in the world at 0, 0 and go to infinity
            //
            //IMPORTANT: both are on the same scale by default, but zoom will make world coordinates smaller or bigger

            //given a screen position, calculate which Cell is located there
            sf::Vector2i ScreenToCell(sf::Vector2i screen);

            //given a world position, output a cell position
            sf::Vector2i WorldToCell(sf::Vector2f world);

            //takes in a Cell position and returns a Chunk position
            sf::Vector2i CellToChunkPos(int x, int y);

            //given two Cell positions, return whether they're in the same chunk
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