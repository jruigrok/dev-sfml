#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace std;

const int radius = 5;
const int pointCount = 10;
const int cellSize = radius * 2;
const int width = 100;
const int height = 100;
const int depth = 5;
const int screenWidth = width * cellSize;
const int screenHeight = height * cellSize;
const int subSteps = 8;
const float dt = 0.001;
sf::Vector2f g = { 0,100 };

struct Circle {
	sf::CircleShape circle;
	sf::Vector2f pos;
	sf::Vector2f a = {0,0};
	sf::Vector2f oldPos;
	sf::Vector2f drawPos;
	sf::Color color;
	
	Circle(float x, float y, float vx,float vy, sf::Color c) 
		:
		pos(x, y), 
		oldPos(x-vx, y-vy), 
		circle(radius, pointCount), 
		color(c.r, c.g, c.b, c.a)
	{
		circle.setFillColor(color);
		circle.setPosition(pos);
	};
	void setPos(sf::Vector2f Pos) {
		pos = Pos;
		circle.setPosition(pos);
	}

	void movePos(sf::Vector2f Pos) {
		pos += Pos;
		circle.setPosition(pos);
	}

	void updatePos(float dt) {
		sf::Vector2f v = pos - oldPos;
		oldPos = pos;
		movePos(v + a * dt * dt);
		a = { 0,0 };
	}
};

std::vector<Circle> circles;
int grid[width][height][depth];
int gridL[width][height];

void fillGrid() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			gridL[i][j] = 0;
		}
	}
	int l = circles.size();
	int x;
	int y;
	for (int i = 0; i < l; i++) {
		x = (int)floorf(circles[i].pos.x / cellSize);
		y = (int)floorf(circles[i].pos.y / cellSize);
		grid[x][y][gridL[x][y]] = i;
		gridL[x][y]++;
	}
}

void addGravity(sf::Vector2f g) {
	int l = circles.size();
	for (int i = 0; i < l; i++) {
		circles[i].a = g;
	}
}

void collide(int v1[],int v2[], int v1l, int v2l) {
	for (int i = 0; i < v1l; i++) {
		for (int j = 0; j < v2l; j++) {
			if (v1[i] != v2[j]) {
				Circle *ob1 = &circles[v1[i]];
				Circle *ob2 = &circles[v2[j]];
				float dx = ob2->pos.x - ob1->pos.x;
				float dy = ob2->pos.y - ob1->pos.y;
				float d = sqrt(dx * dx + dy * dy);
				if (d < cellSize && d != 0) {
					float overlap = d - cellSize;
					sf::Vector2f dir = (ob2->pos - ob1->pos) / d;

					ob1->movePos(overlap * dir / (float)2);
					ob2->movePos(-overlap * dir / (float)2);
				}
			}
		}
	}
}

void searchGrid() {
	for (int i = 1; i < width - 1; i++) {
		for (int j = height - 2; j > 0; j--) {
			int *v1 = grid[i][j];
			int v1l = gridL[i][j];
			if (v1l != 0) {
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						int *v2 = grid[i + dx][j + dy];
						int v2l = gridL[i + dx][j + dy];
						if (v2l != 0) {
							collide(v1, v2, v1l, v2l);
						}
					}
				}
			}
		}
	}
}

void boundingBox() {
	int l = circles.size();
	for (int i = 0; i < l; i++) {
		if (circles[i].pos.x <= cellSize) {
			circles[i].pos.x = cellSize;
		}
		if (circles[i].pos.x >= screenWidth - cellSize * 2) {
			circles[i].pos.x = screenWidth - cellSize * 2;
		}
		if (circles[i].pos.y <= cellSize) {
			circles[i].pos.y = cellSize;
		}
		if (circles[i].pos.y >= screenHeight - cellSize * 2) {
			circles[i].pos.y = screenHeight - cellSize * 2;
		}
	}
}


void update(float dt) {
	// apply gravity
	addGravity(g);

	// update positions
	int l = circles.size();
	for (int i = 0; i < l; i++) {
		circles[i].updatePos(dt);
	}

	boundingBox();
}