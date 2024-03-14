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

    width = size.x;
    height = size.y;

    sprite = new sf::Sprite();
    sprite->setTexture(*tex);

    sprite->setScale(sf::Vector2f(scale, scale));

    ResetOrigin();
}

SpriteRenderer::SpriteRenderer(sf::Sprite* sprite)
{
    this->sprite = sprite;

    sf::Vector2u size = sprite->getTexture()->getSize();

    width = size.x;
    height = size.y;
}

SpriteRenderer::~SpriteRenderer()
{
    //cleanup that texture in memory, those aren't cheap you know!
    const sf::Texture* tex = sprite->getTexture();
    delete tex;
    delete sprite;
}

void SpriteRenderer::Awake()
{

}

void SpriteRenderer::Update()
{

}

void SpriteRenderer::Draw(sf::RenderWindow* target)
{
    //sprite->setPosition(gameObject->position.x, gameObject->position.y );
    sprite->setPosition(gameObject->position);
    sprite->setRotation(rotato);
    target->draw(*sprite);

    sf::CircleShape origin(10.f);
    origin.setOrigin(gameObject->position);
    origin.setOutlineColor(sf::Color::Blue);
    origin.setFillColor(sf::Color::Transparent);
    target->draw(origin);
}

std::string SpriteRenderer::GetComponentID()
{
    return "SpriteRenderer";
}

void SpriteRenderer::ResetOrigin()
{
    sprite->setOrigin(sf::Vector2f(width/(2), height/(2)));
}