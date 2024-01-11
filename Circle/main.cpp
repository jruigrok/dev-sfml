#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <Timing.hpp>
#include <System.hpp>
#include <Solver.hpp>
#include <functional>
#include <chrono>


int main(int argc, char* argv[]) {
    const uint32_t width = 600;
    const uint32_t height = 1800;
    const uint32_t depth = 6;
    const uint32_t cellSize = 30;
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
    ViewPort viewPort(statesV, { 0,0 }, 0.5);
    //Timing timing;

    std::string circlePngFilepath = std::string(ARTIFACTS_PATH) + "circle.png";

    if (!circleImg.loadFromFile(circlePngFilepath)) {
        throw std::runtime_error("Error file not found " + circlePngFilepath);
    }

    sf::Shader shader;
    
    if (sf::Shader::isAvailable())
    {
        // load only the vertex shader
        const std::string vertShaderPath = "../Shaders/bloomV.glsl";
        const std::string fragShaderPath = "../Shaders/bloomF.glsl";
        if (!shader.loadFromFile(vertShaderPath, fragShaderPath))
        {
            throw std::runtime_error("Error file not found " + vertShaderPath + " and " + fragShaderPath);
        }
    }
    


    circleImg.setSmooth(true);
    circleImg.generateMipmap();
    objectStates.texture = &circleImg;
    //objectStates.shader = &shader;
    //shader.setUniform("texture", sf::Shader::CurrentTexture);
    window.setFramerateLimit(frameLimit);
    Grid grid(width, height, depth, cellSize, dt);
    System system(subSteps, &grid, window, viewPort);
    

    system.setBoarderState(&boarderStates);
    system.setObjState(&objectStates);
    shader.setUniform("image", sf::Shader::CurrentTexture);
    shader.setUniform("horizontal", true);

    system.makeRope(100, 100, 50, 1.0, sf::Color::Red);
    system.makeRigidBody(200, 200, 50, 30, 1.0, sf::Color::Cyan);
    Solver solver(&system, &grid, &window);

    //Circle c({ 100,20 }, { 0,0 });
    //grid.addElementToGrid(&c);
    //Circle c2({ 100,30 }, { 0,0 });
    //grid.addElementToGrid(&c2);
    while(window.isOpen()) {
        sf::Event Event;
        while (window.pollEvent(Event)) {
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
        
        window.clear();
        system.handleInputs();
        //timing.tick();
        system.makeVAs();
        system.drawObjects();
        system.drawBoarder();

        
        
        
        
        window.display();
    }
    
    return 0;
}