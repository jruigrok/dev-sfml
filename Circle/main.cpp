#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Timing.hpp>
#include <Setup.hpp>


int main(int argc, char* argv[]) {
    const uint32_t screenWidth = width * (uint32_t)cellSize;
    const uint32_t screenHeight = height * (uint32_t)cellSize;
    const uint32_t frameLimit = 60;

    System system(0.001f, 8);


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
    //sf::Transform scale;
    //scale.scale(5, 5);
    //states.transform = scale;
    

    // Output project version
    std::cout << "Version: " << PROJECT_VERSION_MAJOR << "."
         << PROJECT_VERSION_MINOR << std::endl;
	
    Timing timing;
    // Create a window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML works!");
    // Set frame limit
    window.setFramerateLimit(frameLimit);
    
    
    sf::Font font;
    font.loadFromFile("Fonts/arial.ttf");
    // Creae a text
    sf::Text text("hello", font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);

    Circle c1({ 200,100 }, { 0.5,0 });
    
    fillGrid();

    testGrid();
    
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
                    c1.pos = { 200,100 };
                    c1.oldPos.y = c1.pos.y;
                    for (uint32_t i = 0; i < 10; i++) {
                        c1.pos += {0, cellSize};
                        c1.oldPos.y = c1.pos.y;
                        circles.emplace_back(c1);
                    }
                }
                if (Event.key.code == sf::Keyboard::Q) {
                    cout << "objs: " << circles.size() << endl;
                }
            }
        }
        timing.tick();
        window.clear();
        if (circles.size() < 10000) {
            c1.pos = { 200,100 };
            c1.oldPos.y = c1.pos.y;
            for (uint32_t i = 0; i < 10; i++) {
                c1.pos += {0, cellSize};
                c1.oldPos.y = c1.pos.y;
                circles.push_back(c1);
            }
        }
        
        system.nextFrame();
        
        window.draw(system.quad, states);
        //text.setString(timing.getCountString());
        //window.draw(text);
        window.display();
    }
    
    return 0;
}