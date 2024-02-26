#include <SFML/Graphics.hpp>

#include "KGenerics/LinkedList.h"
#include "CaveSand/CaveSand.h"
#include "Game/GameObject.h"
#include "Game/Components/PlayerMovement.h"
#include "CaveSand/Camera.h"
#include "Game/Components/HitPoints.h"
#include "Game/Components/SpriteRenderer.h"

#include <iostream>

using namespace Segratha;

/// @brief Initializes the Camera given the parameters in the CAMERA class
/// @param window the window to draw to
void InitializeCamera(sf::RenderWindow& window)
{
    //set zoom
    CAMERA::zoom = 3.f;

    CAMERA::view = sf::View();
    CAMERA::ChangeCenter(sf::Vector2f(CHUNK_SIZE * CELL_SIZE * 2 + 32 * CELL_SIZE, CHUNK_SIZE * CELL_SIZE * 1.5f));
    CAMERA::ChangeSize(CAMERA::ASPECT_RATIO * CAMERA::zoom * CAMERA::GAME_SIZE, CAMERA::zoom * CAMERA::GAME_SIZE);

    window.setView(CAMERA::view);
}

void AwakenAllGameObjects(LinkedList<GameObject*> list)
{
        Node<GameObject*>* node = list.head;
        do
        {
            GameObject* g = node->data;

            g->Awake();
            
        } while(node->next != nullptr);
}

void UpdateAllGameObjects(LinkedList<GameObject*> list, sf::RenderWindow* target)
{
        Node<GameObject*>* node = list.head;
        while(node != nullptr)
        {
            GameObject* g = node->data;

            if(g->Garbage())
            {
                Node<GameObject*>* garbNode = node;
                node = node->next;
                list.RemoveNode(garbNode); //this removes the node and fully deletes the gameobject btw
                continue;
            }

            g->Update();
            g->Draw(target);

            node = node->next;
        } 
}

/// @brief Creates a new GameObject, adds it to the list, and calls "Awake()"
/// @param list the list of gameobjects to add this to
/// @return Pointer to the new Object
GameObject* Instantiate(LinkedList<GameObject*> list)
{
    GameObject* g = new GameObject();
    list.AddNode(g);
    g->Awake();

    return g;
}

int main()
{
    //INIT SCREEN---------------------------------------------------------------------
    sf::RenderWindow window(sf::VideoMode(CAMERA::ASPECT_RATIO * CAMERA::SCREEN_SIZE, CAMERA::SCREEN_SIZE), "Segratha");
    InitializeCamera(window); //pass screen through funny setup
    //--------------------------------------------------------------------------------

    //init CaveSand engine
    CaveSand* manager = CaveSand::GetInstance();

    //init Time stuff for deltatime
    KyleTime::GetInstance();

    //DEBUG---------------------------------------------------------------------------
    sf::Font font;
    font.loadFromFile("minecrap.ttf");

    sf::Text fps("FPS: " + std::to_string(1.f / KyleTime::DeltaTime()), font, 50);
    sf::Text mousePos("MOUSE: ", font, 50);
    //--------------------------------------------------------------------------------


    //SET UP PLAYER-------------------------------------------------------------------
    GameObject* player = new GameObject();

    player->AddComponent(new BasicPhysics(sf::Vector2i(8, 8)));
    player->AddComponent(new PlayerMovement());
    player->AddComponent(new HitPoints(100, false));
    player->AddComponent(new SpriteRenderer("src/Sprites/Player/gonk.png"));
    //--------------------------------------------------------------------------------

    LinkedList<GameObject*> SceneObjects(player);

    //call awake on every GameObject at the start (so that any in the scene at the very beginning get their call)
    //NOTE: when creating a new GameObject, make sure to call Awake() lol
    AwakenAllGameObjects(SceneObjects);

    float timer = 0.005f;
    while (window.isOpen())
    {
        CAMERA::ChangeCenter(player->position);
        KyleTime::UpdateDelta(); //Update Deltatime calculation

        window.setView(CAMERA::view); //update camera position

        //CLOSE WINDOW EVENT <VITAL> ------------------------------------------------------------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //---------------------------------------------------------------------------------------------------------------------------------------


        window.clear(sf::Color(155, 204, 161)); //clear window for rendering

        manager->FullRun(&window, timer); //update the CaveSand engine and draw it

        UpdateAllGameObjects(SceneObjects, &window); //update every GameObject in the scene

        //BRUSH CODE -----------------------------------------------------------------------------------
        
        static int brush = 0;        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            brush = 0;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            brush = 1;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            brush = 2;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            brush = 3;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
        {
            brush = 4;
        }

        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        mouse = manager->ScreenToCell(mouse);

        //show what cell the mouse is hovering over
        sf::RectangleShape mouseSelect = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        mouseSelect.setPosition(sf::Vector2f(mouse.x, mouse.y)*CELL_SIZE);
        mouseSelect.setFillColor(sf::Color::Transparent);
        mouseSelect.setOutlineColor(sf::Color::Red);
        mouseSelect.setOutlineThickness(5);
        window.draw(mouseSelect);

        mousePos.setString("MOUSE POS: " + std::to_string(mouse.x) + ", " + std::to_string(mouse.y));
        mousePos.setPosition(CAMERA::view.getCenter() - sf::Vector2f(CAMERA::GetScreenWidth()/2, CAMERA::GetScreenHeight()/2 + 50));
        window.draw(mousePos);

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            static int iter = 0;

            //sf::Vector2i mouse = sf::Mouse::getPosition(window);

            //mouse = manager->ScreenToCell(mouse);

            int brushSize = 5;
            cell_type t = AIR;

            switch(brush)
            {
                case 0:
                    t = AIR;
                    break;
                case 1:
                    t = SOLID;
                    break;
                case 2:
                    t = SAND;
                    break;
                case 3:
                    t = WATER;
                    break;
                case 4:
                    t = GAS;
                    break;
            }

            for(int x = -brushSize; x <= brushSize; x++)
                        for(int y = -brushSize; y <= brushSize; y++)
                            manager->Set(mouse.x + x, mouse.y + y, Cell(t));
        }

        //-----------------------------------------------------------------------------------------------------------------------------------------

        //FPS COUNTER------------------------------------------------------------------------------------------------------------------------------
        fps.setPosition(CAMERA::view.getCenter() - sf::Vector2f(CAMERA::GetScreenWidth()/2, CAMERA::GetScreenHeight()/2));
        window.draw(fps);

        static float fpsTimer = 0;
        static float fpsMoment = 0;
        static float numFrames = 0;
        if(fpsTimer > 1)
        {
            fps.setString("FPS: " + std::to_string(fpsMoment / numFrames));
            numFrames = 0;
            fpsMoment = 0;
            fpsTimer = 0;
        }
        else
        {
            numFrames++;
            fpsMoment += 1.f / KyleTime::DeltaTime();
        }
        fpsTimer += KyleTime::DeltaTime();
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        window.display();
    }

    delete manager;
    delete KyleTime::GetInstance();

    return 0;
}