#include <SFML/Graphics.hpp>

#include "CaveSand/CaveSand.h"
#include "Game/GameObject.h"
#include "Game/Components/CellCollider.h"
#include "CaveSand/Camera.h"

#include <iostream>

using namespace Segratha;

int main()
{
    // for(int i = 0; i > -CHUNK_SIZE * 2; i--)
    // {
    //     std::cout << "i: " << i << " i \% CHUNK_SIZE: " << (i % CHUNK_SIZE) + CHUNK_SIZE - 1 << std::endl;
    // }


    //set zoom
    CAMERA::zoom = 3.f;

    sf::RenderWindow window(sf::VideoMode(CAMERA::ASPECT_RATIO * CAMERA::SCREEN_SIZE, CAMERA::SCREEN_SIZE), "Segratha");

    CAMERA::view = sf::View();
    CAMERA::ChangeCenter(sf::Vector2f(CHUNK_SIZE * CELL_SIZE * 2 + 32 * CELL_SIZE, CHUNK_SIZE * CELL_SIZE * 1.5f));
    CAMERA::ChangeSize(CAMERA::ASPECT_RATIO * CAMERA::zoom * CAMERA::GAME_SIZE, CAMERA::zoom * CAMERA::GAME_SIZE);

    window.setView(CAMERA::view);

    CaveSand* manager = CaveSand::GetInstance();
    // manager->LoadAt(-1, 0);
    // manager->LoadAt(0, 0);
    // manager->LoadAt(-1, 1);
    // manager->LoadAt(1, 0);
    
    //manager->Set(0, 50, Cell(SOLID));

    KyleTime::GetInstance();

    sf::Font font;
    font.loadFromFile("minecrap.ttf");

    sf::Text fps("FPS: " + std::to_string(1.f / KyleTime::DeltaTime()), font, 50);

    GameObject player;

    player.AddComponent(new CellCollider(sf::Vector2i(5, 5)));
    
    sf::Text mousePos("MOUSE: ", font, 50);

    float timer = 0.005f;
    while (window.isOpen())
    {
        CAMERA::ChangeCenter(player.position);
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


        window.clear(); //clear window for rendering

        manager->FullRun(&window, timer); //update the CaveSand engine and draw it

        player.Update(); //update player
        player.Draw(&window); //draw player character to screen

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