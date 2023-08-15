#include "config.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <Physics.hpp>

int main(int argc, char* argv[]) {
    
    // Output project version
    std::cout << "Version: " << PROJECT_VERSION_MAJOR << "."
         << PROJECT_VERSION_MINOR << std::endl;
	
    // Create a window
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
    //sf::CircleShape shape(r);
    //shape.setPosition(pos);
    //shape.setFillColor(sf::Color::Green);
    std::vector<circleDef> circles(10);

    //for (auto& c : circles)
    for(auto i = 0; i < circles.size(); i++)
    {
        circles[i].setPos(i * 30, i * 30);
    }

    // Handle closing the window
    while(window.isOpen()) {
        sf::Event Event;
        while(window.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed)
                window.close();
            if (Event.type == sf::Event::KeyPressed) {
                if (Event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }
        window.clear();
        for (auto& c : circles) {
            window.draw(c.circle);
        }
        window.display();
    }
    
    return 0;
}
