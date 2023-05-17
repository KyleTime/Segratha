#ifndef CHUNKREND_H
#define CHUNKREND_H

#include <SFML/Graphics.hpp>
#include "Chunk.h"

namespace Segratha
{
    const float CELL_SIZE = 10;

    class ChunkRend
    {
        private:
            sf::VertexArray vert;
            Chunk* bound;

        public:
            ChunkRend();

            /// @brief Given a chunk, align this with it
            /// @param c 
            void Bind(Chunk* c);

            /// @brief unbinds the render chunk
            void Unbind();

            /// @brief Renders the last bound chunk onto the screen (see Bind(Chunk*))
            /// @param target The window to render to
            void ChunkDraw(sf::RenderWindow* target);
    };
}

#endif