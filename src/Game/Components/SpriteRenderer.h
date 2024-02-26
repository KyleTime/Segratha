#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../Component.h"

namespace Segratha
{
    class SpriteRenderer : public Component
    {
        private:
            sf::Sprite sprite;
            //width in pixels of the sprite
            int width;
            //height in pixels of the sprite
            int height;

        public:
            SpriteRenderer(std::string path);
            SpriteRenderer(sf::Sprite path);

            void Awake() override;

            void Update() override;
            void Draw(sf::RenderWindow* target) override;

            std::string GetComponentID() override;
    };
}

#endif