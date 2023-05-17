#ifndef CHUNKREND_H
#define CHUNKREND_H

#include <SFML/Graphics.hpp>
#include "Chunk.h"

namespace Segratha
{
    const float CELL_SIZE = 10;
    const int REND_SIZE = CHUNK_SIZE / 4;

    class ChunkRend
    {
        private:
            sf::VertexArray vert;
            sf::Transform transform;
            Chunk* bound;
            bool active = false;

        public:
            ChunkRend();

            /// @brief Given a render chunk position, align self to that position 
            void Bind(Chunk* c);

            /// @brief until the next Bind(), the chunk is inactive
            void Unbind();

            /// @brief Renders the last bound chunk onto the screen (see Bind(Chunk*))
            /// @param target The window to render to
            void ChunkDraw(sf::RenderWindow* target);
    };
}

#endif