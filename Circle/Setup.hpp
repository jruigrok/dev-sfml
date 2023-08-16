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
	sf::Vector2f a = {0,0};
	sf::Vector2f oldPos;
	sf::Color color;
	
	Circle() {};
	Circle(float x, float y, sf::Color c) :pos(x, y), oldPos(x, y), circle(radius), color(c.r, c.g, c.b, c.a) {
		circle.setFillColor(color);
		circle.setPosition(pos);
	};
	void setPos(sf::Vector2f Pos) {
		pos = Pos;
		circle.setPosition(Pos);
	}

	void updatePos(float dt) {
		sf::Vector2f v = pos - oldPos;
		oldPos = pos;
		pos += v + a * dt * dt;
		a = { 0,0 };
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

void addGravity(sf::Vector2f g) {
	int l = circles.size();
	for (int i = 0; i < l; i++) {
		circles[i].a = g;
	}
}

void collide(std::vector<int> v1, std::vector<int> v2) {
	int v1l = v1.size();
	int v2l = v2.size();
	for (int i = 0; i < v1l; i++) {
		for (int j = 0; j < v2l; j++) {
			if (v1[i] != v2[j]) {
				Circle *ob1 = &circles[v1[i]];
				Circle *ob2 = &circles[v2[j]];
				float dx = ob2->pos.x - ob1->pos.x;
				float dy = ob2->pos.y - ob1->pos.y;
				float d = sqrt(dx * dx + dy * dy);
				if (d < cellSize) {
					float overlap = d - cellSize;
					sf::Vector2f dir = (ob2->pos - ob1->pos) / d;
					ob1->pos += overlap * dir / (float)2;
					ob2->pos += -overlap * dir / (float)2;
				}
			}
		}
	}
}

void searchGrid() {
	for (int i = 1; i < width - 1; i++) {
		for (int j = height - 2; j > 0; j--) {
			std::vector<int> v1 = grid[i][j];
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					std::vector<int> v2 = grid[i + dx][j + dy];
					collide(v1, v2);
				}
			}
		}
	}
}