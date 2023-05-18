#ifndef CHUNKREND_H
#define CHUNKREND_H

#include <SFML/Graphics.hpp>
#include "Chunk.h"

namespace Segratha
{
    const float CELL_SIZE = 10;
    const int DIVISOR = 4; //how many times will we split each chunk for rendering?
    const int REND_SIZE = CHUNK_SIZE / DIVISOR;

    class ChunkRend
    {
        private:
            sf::VertexArray vert;
            sf::Transform transform;
            Chunk* bound;
            sf::Vector2i position; //position in render chunk coordinates
            bool active = false;

        public:
            ChunkRend();

            /// @brief Given a render chunk position, align self to that position 
            void Bind(int x, int y, Chunk* c);

            /// @brief until the next Bind(), the chunk is inactive
            void Unbind();

            /// @brief Renders the last bound chunk onto the screen (see Bind(Chunk*))
            /// @param target The window to render to
            void ChunkDraw(sf::RenderWindow* target);
    };
}

#endif