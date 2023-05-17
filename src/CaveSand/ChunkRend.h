#ifndef CHUNKREND_H
#define CHUNKREND_H

#include <SFML/Graphics.hpp>
#include "Chunk.h"

namespace Segratha
{
    const int REND_FACTOR = 8; //number of render chunks in each chunk
    const float CELL_SIZE = 10; //size of a single cell in pixels

    class ChunkRend
    {
        private:
            sf::VertexArray vert; //vertices for rendering
            Chunk* bound; //the chunk that's bound to this object
            int rendStart; //where we start rendering in the bound chunk

        public:
            ChunkRend();

            /// @brief Given a chunk, align this with it
            /// @param c 
            void Bind(Chunk* c, sf::Vector2f rendPos, int startPos);

            /// @brief unbinds the render chunk
            void Unbind();

            /// @brief Renders the last bound chunk onto the screen (see Bind(Chunk*))
            /// @param target The window to render to
            void ChunkDraw(sf::RenderWindow* target);
    };
}

#endif