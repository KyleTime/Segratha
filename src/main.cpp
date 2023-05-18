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
 
        //-------------------------------------------------------------------------------------------------------------------------------

        //Funny debug

        //grab view
        sf::View view = CAMERA::view;

        sf::Vector2f camPos = view.getCenter(); //grab the camera position
        sf::Vector2f camSize = view.getSize(); //grab camera size

        //sf::Vector2i min = manager->ScreenToCell(sf::Vector2i(camPos.x - camSize.x, camPos.y - camSize.y)); //calculate where cell at top left of camera is located
        //sf::Vector2i max = manager->ScreenToCell(sf::Vector2i(camPos.x + camSize.x, camPos.y + camSize.y)); //calculate where cell at bottom right of camera is located

        sf::Vector2i min = (sf::Vector2i(camPos.x - camSize.x/2, camPos.y - camSize.y/2)); //calculate where cell at top left of camera is located
        sf::Vector2i max = (sf::Vector2i(camPos.x + camSize.x/2, camPos.y + camSize.y/2)); //calculate where cell at bottom right of camera is located

        sf::Vector2i minPos = sf::Vector2i(min.x/CELL_SIZE, min.y/CELL_SIZE); //get chunk pos of min
        sf::Vector2i maxPos = sf::Vector2i(max.x/CELL_SIZE, max.y/CELL_SIZE); //get chunk pos of max

        std::string string = "min screen: ";
        string += std::to_string(minPos.x);
        string += ", ";
        string += std::to_string(minPos.y);
        string += " max screen: ";
        string += std::to_string(maxPos.x);
        string += ", ";
        string += std::to_string(maxPos.y);
        string += "\nmouse:";
        sf::Vector2i mousePos = manager->ScreenToCell(sf::Mouse::getPosition(window));
        string += std::to_string(mousePos.x);
        string += ", ";
        string += std::to_string(mousePos.y);
        
        //std::cout << "CAM POS: " << camPos.x << ", " << camPos.y << " CAM SIZE: " << camSize.x << ", " << camSize.y << std::endl;

        sf::Text debug(string, font, 50);
        debug.setPosition(CAMERA::view.getCenter() - sf::Vector2f(CAMERA::GetScreenWidth()/2, CAMERA::GetScreenHeight()/2) - sf::Vector2f(0, -1000));
        window.draw(debug);

        sf::RectangleShape funny(sf::Vector2f(10, 10));
        funny.setPosition(0, 0);
        funny.setOrigin(5.f, 5.f);

        window.draw(funny);

        sf::RectangleShape funny1(sf::Vector2f(20, 20));
        funny1.setPosition(min.x, min.y);

        window.draw(funny1);

        sf::RectangleShape funny2(sf::Vector2f(20, 20));
        funny2.setPosition(max.x, max.y);
        funny2.setOrigin(20.f, 20.f);

        window.draw(funny2);
        //--------------------------------------------------------------------------------------------------------------------------------

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