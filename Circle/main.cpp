#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Timing.hpp>
#include <System.hpp>
#include <Examples.hpp>
#include <functional>


int main(int argc, char* argv[]) {
    const uint32_t width = 402;
    const uint32_t height = 200;
    const uint32_t cellSize = 4;
    const uint32_t screenWidth = width * (uint32_t)cellSize;
    const uint32_t screenHeight = height * (uint32_t)cellSize;
    const uint32_t frameLimit = 60;


    std::string circlePngFilepath = std::string(ARTIFACTS_PATH) + "circle.png";

    sf::Vector2i mouse;
    sf::Texture circleImg;
    if (!circleImg.loadFromFile(circlePngFilepath)) {
        throw std::runtime_error("Error file not found " + circlePngFilepath);
    }
    circleImg.setSmooth(true);
    circleImg.generateMipmap();

    sf::RenderStates states;
    states.texture = &circleImg;
    

    // Output project version
    /*std::cout << "Version: " << PROJECT_VERSION_MAJOR << "."
         << PROJECT_VERSION_MINOR << std::endl;*/
	
    Timing timing;
    // Create a window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML works!");
    // Set frame limit
    //window.setFramerateLimit(frameLimit);
    
    
    sf::Font font;
    font.loadFromFile(std::string(ARTIFACTS_PATH) + "Fonts/arial.ttf");
    // Creae a text
    sf::Text text("hello", font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);

    Circle c1({ 200,500 }, { 1,0 });

    std::vector<Link> links;

    Grid<Circle> grid(width, height, 5, cellSize);
    System system(0.001f, 8, grid, window, states);


    system.makeBoarder(cellSize);


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
                if (Event.key.code == sf::Keyboard::Return) {
                    c1.setPos({ 200,100 });
                    for (uint32_t i = 0; i < 50; i++) {
                        c1.movePos({ 0, cellSize });
                        grid.addElementToGrid(c1);
                    }
                }
                if (Event.key.code == sf::Keyboard::Q) {
                    cout << "objs: " << grid.size() << endl;
                }
            }
        }
        timing.tick();
        window.clear();
        if (grid.size() < 20000) {
            c1.setPos({ 200, 500 });
            for (uint32_t i = 0; i < 25; i++) {
                //c1.pos += {0, cellSize};
                c1.movePos({ 0, cellSize });
                //c1.setVelocity({ 1,0 });
                grid.addElementToGrid(c1);
            }
        }
        
        system.updatePos();
        system.drawFrame();
        window.display();
    }
    
    return 0;
}