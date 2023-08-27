#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Timing.hpp>
#include <System.hpp>


int main(int argc, char* argv[]) {
    const uint32_t width = 400;
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
    std::cout << "Version: " << PROJECT_VERSION_MAJOR << "."
         << PROJECT_VERSION_MINOR << std::endl;
	
    Timing timing;
    // Create a window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML works!");
    // Set frame limit
    window.setFramerateLimit(frameLimit);
    
    
    sf::Font font;
    font.loadFromFile(std::string(ARTIFACTS_PATH) + "Fonts/arial.ttf");
    // Creae a text
    sf::Text text("hello", font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);

    Circle c1({ 200,100 }, { 1,0 });
    
    

    //testGrid();

    //std::vector<Circle> circles;
    std::vector<Link> links;

    Grid<Circle> grid(width, height, 5, cellSize);


    System system(0.001f, 8, grid, window, states);

    /*circles.push_back(c1);
    circles.push_back(c2);

    Link l1(0, 1, c1.pos, c2.pos);
    links.push_back(l1);
    circles.push_back(c3);*/

    //system.makeRigidBody(100, 100, 3, 3);
    //system.makeRigidBody(150, 400, 40, 40,0.5);
    //system.makeRope(400, 100, 20, 0.8);

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
                    for (uint32_t i = 0; i < 10; i++) {
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
        if (grid.size() < 10000) {
            c1.setPos({ 200, 100 });
            for (uint32_t i = 0; i < 10; i++) {
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