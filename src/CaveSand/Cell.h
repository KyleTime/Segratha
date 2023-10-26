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

            /// @brief Gets the CaveSand Engine.
            /// @return A pointer to the CaveSand Engine.
            CaveSand* GetSand();

            Chunk* ChunkSteppy(int& x, int& y, Chunk* c);

            //move by (xM, yM) in the current chunk given rel coords (x, y)
            //WARNING: Assumes that you're not moving the cell by more than 1 chunk in distance. You shouldn't be doing that anyway, we'd have weird multithreading issues.
            //also, there's a funny optional bool on the end, setting it to true will have the move function ignore whether the destination is solid
            bool Move(int& x, int& y, int xm, int ym, Chunk* c, bool replace = false);
            //update functions for different types

            //Updates a cell as if it were sand
            void SandUpdate(int x, int y, Chunk* c);

            //Updates a cell as if it were water
            void WaterUpdate(int x, int y, Chunk* c);
    };
}

#endif