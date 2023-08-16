#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace std;

const int radius = 5;
const int cellSize = radius * 2;
const int width = 100;
const int height = 100;
const int screenWidth = width * cellSize;
const int screenHeight = height * cellSize;

struct Circle {
	sf::CircleShape circle;
	sf::Vector2f pos;
	sf::Color color;
	Circle() {};
	Circle(float x, float y, sf::Color c) :pos(x, y), circle(radius), color(c.r, c.g, c.b, c.a) {
		circle.setFillColor(color);
		circle.setPosition(pos);
	};
	void setPos(float x, float y) {
		pos.x = x;
		pos.y = y;
		circle.setPosition(pos);
	}
};

std::vector<Circle> circles;
std::vector<int> grid[width][height];

void fillGrid() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			grid[i][j].clear();
		}
	}
	int l = circles.size();
	int x;
	int y;
	for (int i = 0; i < l; i++) {
		x = floorf(circles[i].pos.x / cellSize);
		y = floorf(circles[i].pos.y / cellSize);
		grid[x][y].push_back(i);
	}
}