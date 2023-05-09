#include <SFML/Graphics.hpp>

#include "CaveSand/CaveSand.h"
#include "PlayerCore.h"

#include <iostream>

using namespace Segratha;

int main()
{
    //set zoom
    CAMERA::zoom = 3.f;

    sf::RenderWindow window(sf::VideoMode(CAMERA::ASPECT_RATIO * CAMERA::SCREEN_SIZE, CAMERA::SCREEN_SIZE), "Segratha");

    CAMERA::view = sf::View();
    CAMERA::ChangeCenter(sf::Vector2f(CHUNK_SIZE * CELL_SIZE * 2 + 32 * CELL_SIZE, CHUNK_SIZE * CELL_SIZE * 1.5f));
    CAMERA::ChangeSize(CAMERA::ASPECT_RATIO * CAMERA::zoom * CAMERA::SCREEN_SIZE, CAMERA::zoom * CAMERA::SCREEN_SIZE);

    window.setView(CAMERA::view);

    CaveSand* manager = CaveSand::GetInstance();
    for(int x = 0; x < 4; x++)
        for(int y = 0; y < 3; y++)
        {
            manager->LoadAt(x, y);
        }

    KyleTime* kTime = KyleTime::GetInstance();

    sf::Font font;
    font.loadFromFile("minecrap.ttf");

    //for(int x = 0; x < 4; x++)
    //    for(int y = 0; y < 3; y++)
    //    {
    //        manager->UnLoadAt(x, y);
    //    }

    sf::Text fps("FPS: " + std::to_string(1.f / KyleTime::DeltaTime()), font, 50);

    manager->Set(32, 48, Cell(SAND));
    manager->Set(64 + 32, 48, Cell(SAND));

    PlayerCore player;

    float timer = 0.01f;
    while (window.isOpen())
    {
        KyleTime::UpdateDelta();

        player.Update();

        window.setView(CAMERA::view);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        manager->FullRun(&window, timer);

        static int brush = 0;        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
        {
            brush = 0;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            brush = 1;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            brush = 2;
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            static int iter = 0;

            sf::Vector2i mouse = sf::Mouse::getPosition(window);

            mouse = manager->ScreenToCell(mouse);

            switch(brush)
            {
                case 0:
                    manager->Set(mouse.x, mouse.y, Cell(SOLID));
                    break;
                case 1:
                    manager->Set(mouse.x, mouse.y, Cell(SAND));
                    break;
                case 2:
                    manager->Set(mouse.x, mouse.y, Cell(AIR));
                    break;
            }
            /*
            Cell* p = manager->GetCellAt(mouse.x, mouse.y);
            
            if(p != nullptr && !p->isAir())
            {
                switch(iter % 4)
                {
                    case 0:
                        p->color = sf::Color::Blue;
                        break;
                    case 1: 
                        p->color = sf::Color::Red;
                        break;
                    case 2:
                        p->color = sf::Color::Green;
                        break;
                    case 3:
                        p->color = sf::Color::Magenta;
                        break;
                }

                //iter++;
            } */
        }

        manager->DrawAll(&window);

        player.Draw(&window);


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

        window.display();
    }

    delete manager;
    delete kTime;

    return 0;
}