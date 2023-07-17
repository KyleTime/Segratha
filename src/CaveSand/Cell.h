#ifndef CELLHEADER_H
#define CELLHEADER_H
#include <SFML/Graphics.hpp>
#include <memory>

#include <iostream>

namespace Segratha
{
    enum cell_type : unsigned char { AIR, SOLID, SAND, WATER };

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
            void Update(int xW, int yW, unsigned char cycle);

            //info functions
            bool isAir();
            bool isSolid();
            bool isLiquid();
            bool isGas();

        private:
            //update functions for different types

            //Updates a pixel as if it were sand
            void SandUpdate(int xW, int yW);
    };
}

#endif