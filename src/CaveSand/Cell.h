#ifndef CELLHEADER_H
#define CELLHEADER_H
#include <SFML/Graphics.hpp>
#include <memory>

#include <iostream>

namespace Segratha
{
    class Chunk;
    class CaveSand;

    enum cell_type : unsigned char { AIR, SOLID, SAND, WATER };

    class Cell
    {
        protected:
            unsigned char cycle; //set equal to global cycle after update, stops double updates 
        public:
            static CaveSand* inst;
            cell_type type;
            sf::Color color; //color to render this cell
            Cell();
            Cell(cell_type type);
            ~Cell();
            void Update(int x, int y, unsigned char cycle, Chunk* c);

            //info functions
            bool isAir();
            bool isSolid();
            bool isLiquid();
            bool isGas();

            //returns the integer contained within, incremented every call
            char PixelRand();

        private:
            //general movement functions

            /// @brief Takes in x and y, assuming they are Rel Chunk Coords, determine whether either of them are out of bounds. If one or both are, move chunks until we have the right chunk position, and make sure x and y are within the boundaries of their new chunk.
            /// @param x Rel Chunk Coord for X
            /// @param y Rel Chunk Coord for y
            /// @param c The Chunk we're starting in
            /// @param inst The CaveSand Engine to query
            /// @return Either the Chunk we started with or the new one selected.
            Chunk* ChunkSteppy(int& x, int& y, Chunk* c);

            /// @brief Moves a cell 1 position at a time in the x or y direction, stopping if the targetted cell is solid.
            /// @param x The x position to start at in Rel Chunk Coords (must be within the Chunk "c" and within 0 - (Chunk_Size - 1))
            /// @param y The x position to start at in Rel Chunk Coords (must be within the Chunk "c" and within 0 - (Chunk_Size - 1))
            /// @param xm The amount to move along the x axis (-1 - 1)
            /// @param ym The amount of move along the y axis (-1 - 1) [1 is down, -1 is up]
            /// @param c The Chunk to start in
            /// @param inst The CaveSand Engine to query
            /// @param replace Optional bool that determines whether we care about the target cell being solid
            /// @return Whether the operation was successful
            bool Move(int& x, int& y, int xm, int ym, Chunk* c, bool replace = false);

            /// @brief Checks whether the position is solid relative to the position of the cell.
            /// @param x The x position to start at in Rel Chunk Coords (must be within the Chunk "c" and within 0 - (Chunk_Size - 1))
            /// @param y The x position to start at in Rel Chunk Coords (must be within the Chunk "c" and within 0 - (Chunk_Size - 1))
            /// @param xm The amount to move along the x axis (-1 - 1)
            /// @param ym The amount of move along the y axis (-1 - 1) [1 is down, -1 is up]
            /// @param c The Chunk to start in
            /// @param inst The CaveSand Engine to query
            /// @return Whether targetted cell is solid
            bool SolidAt(int x, int y, int xm, int ym, Chunk* c);

            /// @brief Check All Cells to see if they're solid in a line
            /// @param x Cell X Position
            /// @param y Cell Y Position
            /// @param horiz The distance horizontally
            /// @param c The Chunk we in
            /// @param inst The CaveSand Engine to query
            /// @return the position we end up at
            sf::Vector2i CheckHorizontal(int& x, int& y, int horiz, Chunk* c);

            //update functions for different types

            //Updates a cell as if it were sand
            void SandUpdate(int x, int y, Chunk* c);

            //Updates a cell as if it were water
            void WaterUpdate(int x, int y, Chunk* c);
    };
}

#endif