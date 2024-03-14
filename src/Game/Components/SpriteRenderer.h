#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../Component.h"

namespace Segratha
{
    class SpriteRenderer : public Component
    {
        private:
            sf::Sprite* sprite;
            //width in pixels of the sprite
            int width;
            //height in pixels of the sprite
            int height;


        public:
            float rotato = 0;

            SpriteRenderer(std::string path);
            SpriteRenderer(sf::Sprite* path);
            ~SpriteRenderer();

            void Awake() override;

            void Update() override;
            void Draw(sf::RenderWindow* target) override;

            std::string GetComponentID() override;

            void ResetOrigin();
    };
}

#endif