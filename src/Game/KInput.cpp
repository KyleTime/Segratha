#include "KInput.h"
#include "../CaveSand/CaveSand.h"

using namespace Segratha;

KInput* KInput::inst = nullptr;
LinkedList<GameObject*>* KInput::SceneObjects = nullptr;

KInput::KInput(sf::RenderWindow* window, LinkedList<GameObject*>* head)
{
    this->window = window;
    this->SceneObjects = head;
    KInput::inst = this;
    right = sf::Keyboard::Key::D;
    left = sf::Keyboard::Key::A;
    up = sf::Keyboard::Key::W;
    down = sf::Keyboard::Key::S;
    jump = sf::Keyboard::Key::Space;
}

sf::Vector2i KInput::GetMousePositionScreen()
{
    return sf::Mouse::getPosition(*inst->window);
}

sf::Vector2f KInput::GetMousePositionWorld()
{
    //this is such a fucking stupid way of doing this, but I can't figure it out otherwise
    return CaveSand::GetInstance()->CellToWorld(GetMousePositionCell());
}

sf::Vector2i KInput::GetMousePositionCell()
{
    return CaveSand::GetInstance()->ScreenToCell(GetMousePositionScreen());
}