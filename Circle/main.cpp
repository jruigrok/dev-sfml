#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <Timing.hpp>
#include <System.hpp>
#include <functional>


int main(int argc, char* argv[]) {
    const uint32_t width = 1800;
    const uint32_t height = 280;
    const uint32_t depth = 6;
    const uint32_t cellSize = 3;
    const float dt = 0.001f;
    const uint32_t subSteps = 4;
    const uint32_t screenWidth = 1980 / 2;
    const uint32_t screenHeight = 1080 / 2;
    const uint32_t frameLimit = 60;
    sf::Vector2i mouse;
    sf::Texture circleImg;
    sf::RenderStates objectStates;
    sf::RenderStates states;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Window");
    ViewPort viewPort(states ,objectStates, {-500,0}, 0.5);

    std::string circlePngFilepath = std::string(ARTIFACTS_PATH) + "circle.png";

    if (!circleImg.loadFromFile(circlePngFilepath)) {
        throw std::runtime_error("Error file not found " + circlePngFilepath);
    }
    circleImg.setSmooth(true);
    circleImg.generateMipmap();

    objectStates.texture = &circleImg;
    
    Timing timing;
    
    // Set frame limit
    window.setFramerateLimit(frameLimit);
    
    //sf::Font font;
    //font.loadFromFile(std::string(ARTIFACTS_PATH) + "Fonts/arial.ttf");

    Circle c({ 200,650 }, { -0.1f,1 });
    Circle c1({ 200,100 }, { -0.1f,1 });

    Grid grid(width, height, depth, cellSize, dt);
    System system(subSteps, grid, window, viewPort);

    //system.makeRect(50, 50, c);

    //system.makeRope(100, 100, 10, 0.9f);
    //system.makeRigidBody(200, 500, 10, 15, 0.5f);
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
                if (Event.key.code == sf::Keyboard::Q) {
                    cout << "objs: " << grid.size() << endl;
                }
                
            }
            system.handleEvents(Event);
        }
        timing.tick();
        window.clear();
        system.handleInputs();
        if (grid.size() < 200000 && !system.isPaused()) {
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