#ifndef CELLHEADER_H
#define CELLHEADER_H
#include <SFML/Graphics.hpp>
#include <memory>

#include <iostream>

namespace Segratha
{
    class Chunk;
    class CaveSand;

    enum cell_type : unsigned char { AIR, SOLID, SAND, WATER, GAS };

    class Cell
    {
        protected:
            unsigned char cycle; //set equal to global cycle after update, stops double updates 
        public:
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

            //returns the density of the selected cell
            int GetDensity();

            //returns the integer contained within, incremented every call
            char PixelRand();

        private:
            //general movement functions

            /// @brief Gets the CaveSand Engine.
            /// @return A pointer to the CaveSand Engine.
            CaveSand* GetSand();

            Chunk* ChunkSteppy(int& x, int& y, Chunk* c);

            /// @brief Attempts to move a Cell by (xm, ym)
            /// @param x X Position of Cell
            /// @param y Y Position of Cell
            /// @param xm X MOVE of Cell
            /// @param ym Y MOVE of Cell
            /// @param c Current Chunk
            /// @param useDensity If set to true, elements that are moving downward will fall through less dense material, moving up works the opposite. Horizontal movement goes through less dense materials.
            /// @param replace If set to true, the move will not worry about whether the attempted cell is solid or not.
            /// @return Whether the cell was able to move
            bool Move(int& x, int& y, int xm, int ym, Chunk* c, bool useDensity = false, bool replace = false);

            //update functions for different types

            //Updates a cell as if it were sand
            void SandUpdate(int x, int y, Chunk* c);

            //Updates a cell as if it were water
            void WaterUpdate(int x, int y, int speed, Chunk* c);

            //Updates a cell as if it were gas
            void GasUpdate(int x, int y, int speed, Chunk* c);
    };
}

#endif