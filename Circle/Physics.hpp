#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

sf::Vector2f pos(50, 50);

struct circleDef {
	float r = 40;
	sf::CircleShape circle;
	sf::Vector2f pos;
	sf::Color color;
	circleDef(float x = 50.0, float y = 50.0) :pos(x, y), circle(r), color(95,13,13,255) {
		circle.setFillColor(color);
		circle.setPosition(pos);
	};
	void setPos(float x, float y) {
		pos.x = x;
		pos.y = y;
		circle.setPosition(pos);
	}
};

