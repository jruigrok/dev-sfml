#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <Timing.hpp>
#include <System.hpp>
#include <ViewPort.hpp>
#include <functional>


int main(int argc, char* argv[]) {
    const uint32_t width = 600;
    const uint32_t height = 250;
    const uint32_t depth = 5;
    const uint32_t cellSize = 3;
    const float dt = 0.001f;
    const uint32_t subSteps = 8;
    const uint32_t screenWidth = width * (uint32_t)cellSize;
    const uint32_t screenHeight = height * (uint32_t)cellSize;
    const uint32_t frameLimit = 60;
    sf::Vector2i mouse;
    sf::Texture circleImg;
    sf::RenderStates states;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Window");
    ViewPort viewPort(states, {0,0}, 1);

    cout << "Version: " << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << endl;
    std::string circlePngFilepath = std::string(ARTIFACTS_PATH) + "circle.png";

    
    if (!circleImg.loadFromFile(circlePngFilepath)) {
        throw std::runtime_error("Error file not found " + circlePngFilepath);
    }
    circleImg.setSmooth(true);
    circleImg.generateMipmap();

    
    states.texture = &circleImg;
    
    
    Timing timing;
    // Create a window
    
    // Set frame limit
    window.setFramerateLimit(frameLimit);
    
    //sf::Font font;
    //font.loadFromFile(std::string(ARTIFACTS_PATH) + "Fonts/arial.ttf");

    
    /*camara.move({100,100});
    camara.zoom(2);
    camara.move({ 100,100 });
    camara.zoom(0.5);

    cout << camara.pos.x << endl;*/
    //camara.move({ 100,100 });
    //camara.move({ 100,100 });

    Circle c1({ 200,100 }, { 0.1f,1 });

    Grid grid(width, height, depth, cellSize, dt);
    System system(subSteps, grid, window, states);

    system.makeRope(100, 100, 10, 0.9f);
    system.makeRigidBody(200, 200, 10, 15, 0.5f);
    //system.makeBoarder(cellSize);
    // Handle closing the window
    while(window.isOpen()) {
        sf::Event Event;
        while(window.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed)
                window.close();
            if (Event.type == sf::Event::KeyPressed) {
                if (Event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                /*if (Event.key.code == sf::Keyboard::Return) {
                    c1.setPos({ width * cellSize / 2, 20 });
                    for (uint32_t i = 0; i < 25; i++) {
                        c1.movePos({ cellSize, 0 });
                        grid.addElementToGrid(c1);
                    }
                }*/
                if (Event.key.code == sf::Keyboard::Q) {
                    cout << "objs: " << grid.size() << endl;
                }
                
            }
            viewPort.handleEvents(Event);
        }
        timing.tick();
        window.clear();
        if (grid.size() < 100000) {
            c1.setPos({ width * cellSize / 2, 20 });
            for (uint32_t i = 0; i < 100; i++) {
                c1.movePos({ cellSize, 0 });
                grid.addElementToGrid(c1);
            }
        }
        
        system.updatePos();
        system.drawFrame();
        window.display();
    }
    
    return 0;
}