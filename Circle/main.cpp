#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <Timing.hpp>
#include <Setup.hpp>


int main(int argc, char* argv[]) {
    const int screenWidth = width * cellSize;
    const int screenHeight = height * cellSize;
    const int frameLimit = 60;
    sf::Vector2i mouse;
    sf::Texture circleImg;
    if (!circleImg.loadFromFile("circle.png")) {
        cout << -1 << endl;
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
    font.loadFromFile("Fonts/arial.ttf");
    // Creae a text
    sf::Text text("hello", font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);

    sf::Color red(255, 0, 0, 255);
    Circle c1(200, 100, 1.5, 0,red);
    Circle c2(200, 100+cellSize, 1.5, 0, red);
    Circle c3(200, 100 + cellSize * 2, 1.5, 0, red);
    
    fillGrid();
    
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
                    circles.push_back(c1);
                    circles.push_back(c2);
                    circles.push_back(c3);
                }
                if (Event.key.code == sf::Keyboard::Q) {
                    cout << "objs: " << circles.size() << endl;
                }
            }
        }
        timing.tick();
        window.clear();

        for (int i = 0; i < subSteps; i++) {
            fillGrid();
            searchGrid();
            update(dt/subSteps);
        }
        makeQuads();
        
        window.draw(quad, states);
        //text.setString(timing.getCountString());
        //window.draw(text);
        window.display();
    }
    
    return 0;
}