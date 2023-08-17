#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace std;

const uint32_t radius = 3;
const uint32_t pointCount = 10;
const uint32_t cellSize = radius * 2;
const uint32_t width = 300;
const uint32_t height = 150;
const uint32_t depth = 5;
const uint32_t screenWidth = width * cellSize;
const uint32_t screenHeight = height * cellSize;
const uint32_t subSteps = 8;
const float dt = 0.01;
sf::Vector2f g = { 0,1000 };

struct Circle {
	sf::CircleShape circle;
	sf::Vector2f pos;
	sf::Vector2f a = {0,0};
	sf::Vector2f oldPos;
	sf::Vector2f drawPos;
	sf::Vector2i gridPos;
	sf::Vector2f v;
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
		v = pos - oldPos;
		oldPos = pos;
		movePos(v + a * dt * dt);
		a = { 0,0 };
	}
};

std::vector<Circle> circles;
uint32_t grid[width][height][depth];
uint32_t gridL[width][height];

void fillGrid() {
	for (uint32_t i = 0; i < width; i++) {
		for (uint32_t j = 0; j < height; j++) {
			gridL[i][j] = 0;
		}
	}
	const uint32_t l = circles.size();
	int x;
	int y;
	for (int i = 0; i < l; i++) {
		x = (int)floorf(circles[i].pos.x / cellSize);
		y = (int)floorf(circles[i].pos.y / cellSize);
		if (gridL[x][y] < depth - 1) {
			grid[x][y][gridL[x][y]] = i;
			gridL[x][y]++;
		}
		circles[i].gridPos = { x, y };
	}
}

void addGravity(sf::Vector2f g) {
	const uint32_t l = circles.size();
	for (uint32_t i = 0; i < l; i++) {
		circles[i].a = g;
	}
}

void collide(uint32_t v1[], uint32_t v2[], uint32_t v1l, uint32_t v2l) {
	if (v2l == 0) {
		return;
	}
	else {
		constexpr float response_coef = 1.0f;
		for (uint32_t i = 0; i < v1l; i++) {
			for (uint32_t j = 0; j < v2l; j++) {
				if (v1[i] != v2[j]) {
					Circle* ob1 = &circles[v1[i]];
					Circle* ob2 = &circles[v2[j]];
					const sf::Vector2f d3 = ob2->pos - ob1->pos;
					const float d2 = d3.x * d3.x + d3.y * d3.y;
					if (d2 < cellSize * cellSize && d2 != 0) {
						const float d = sqrt(d2);
						float delta = response_coef * 0.5f * (d - cellSize);
						const sf::Vector2f dir = d3 / d;
						ob1->pos += delta * dir;
						ob2->pos += -delta * dir;
					}
				}
			}
		}
	}
}

void searchGrid() {
	const uint32_t x = width - 2;
	const uint32_t y = height - 2;
	for (uint32_t i = x; i > 1; i--) {
		for (uint32_t j = y; j > 1; j--) {
			const uint32_t v1l = gridL[i][j];
			if (v1l != 0) {
				uint32_t* v1 = grid[i][j];
				collide(v1, grid[i - 1][j - 1], v1l, gridL[i - 1][j - 1]);
				collide(v1, grid[i - 1][j], v1l, gridL[i - 1][j]);
				collide(v1, grid[i - 1][j + 1], v1l, gridL[i - 1][j + 1]);
				collide(v1, grid[i][j - 1], v1l, gridL[i][j - 1]);
				collide(v1, grid[i][j], v1l, gridL[i][j]);
				collide(v1, grid[i][j + 1], v1l, gridL[i][j + 1]);
				collide(v1, grid[i + 1][j - 1], v1l, gridL[i + 1][j - 1]);
				collide(v1, grid[i + 1][j], v1l, gridL[i + 1][j]);
				collide(v1, grid[i + 1][j + 1], v1l, gridL[i + 1][j + 1]);
			}
		}
	}
}

void boundingBox() {
	int l = circles.size();
	for (int i = 0; i < l; i++) {
		if (circles[i].pos.x <= cellSize * 2) {
			circles[i].pos.x = cellSize * 2;
		}
		if (circles[i].pos.x >= screenWidth - cellSize * 2) {
			circles[i].pos.x = screenWidth - cellSize * 2;
		}
		if (circles[i].pos.y <= cellSize * 2) {
			circles[i].pos.y = cellSize * 2;
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