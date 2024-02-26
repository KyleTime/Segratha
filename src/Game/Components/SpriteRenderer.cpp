#include "SpriteRenderer.h"
#include "../GameObject.h"

using namespace Segratha;

SpriteRenderer::SpriteRenderer(std::string path)
{
    sf::Texture* tex = new sf::Texture();
    if(!tex->loadFromFile(path))
    {
        std::cout << "Invalid Path given for image file." << std::endl;
        throw std::bad_exception();
    }

    //we'll assume that everything in here is gonna be non-smooth
    tex->setSmooth(false);

    sf::Vector2u size = tex->getSize();

    float scale = CELL_SIZE;

    width = size.x*scale;
    height = size.y*scale;

    sprite.setTexture(*tex);

    sprite.setScale(sf::Vector2f(scale, scale));
}

SpriteRenderer::SpriteRenderer(sf::Sprite sprite)
{
    this->sprite = sprite;

    sf::Vector2u size = sprite.getTexture()->getSize();

    width = size.x;
    height = size.y;
}

void SpriteRenderer::Awake()
{

}

void SpriteRenderer::Update()
{

}

void SpriteRenderer::Draw(sf::RenderWindow* target)
{
    sprite.setPosition(gameObject->position.x - width/2, gameObject->position.y - height/2);
    target->draw(sprite);
}

std::string SpriteRenderer::GetComponentID()
{
    return "SpriteRenderer";
}