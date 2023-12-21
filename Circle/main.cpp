#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <Timing.hpp>
#include <System.hpp>
#include <functional>

int main(int argc, char* argv[]) {
    const uint32_t width = 450;
    const uint32_t height = 200;
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
    sf::RenderStates boarderStates;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Window");
    std::vector<sf::RenderStates*> statesV = { &boarderStates, &objectStates };
    ViewPort viewPort(statesV, {0,0}, 0.5);
    Timing timing;

    std::string circlePngFilepath = std::string(ARTIFACTS_PATH) + "circle.png";

    if (!circleImg.loadFromFile(circlePngFilepath)) {
        throw std::runtime_error("Error file not found " + circlePngFilepath);
    }
    circleImg.setSmooth(true);
    circleImg.generateMipmap();
    objectStates.texture = &circleImg;
    window.setFramerateLimit(frameLimit);

    Grid grid(width, height, depth, cellSize, dt);
    System system(subSteps, grid, window, viewPort);

    system.makeRope(100, 100, 10, 1);
    //Circle c({ 100,20 }, { 0,0 });
    //grid.addElementToGrid(&c);
    //Circle c2({ 100,30 }, { 0,0 });
    //grid.addElementToGrid(&c2);
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
        
        if (grid.size() < 100000 && !system.isPaused()) {
            float pos = width * cellSize / 2;

            for (uint32_t i = 0; i < 100; i++) {
                Circle c({ pos, 20 }, { 0.3, 1 });
                system.addElement(c);
                pos += cellSize;
            }
        }
        system.updatePos();
        system.drawObjects(objectStates);
        system.drawBoarder(boarderStates);
        window.display();
    }
    
    return 0;
}