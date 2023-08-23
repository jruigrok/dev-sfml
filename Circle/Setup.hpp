#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

const float radius = 2.0f;
const float cellSize = radius * 2.0f;
const uint32_t depth = 5;
const uint32_t width = 400;
const uint32_t height = 200;
const uint32_t screenWidth = width * (uint32_t)std::ceil(cellSize);
const uint32_t screenHeight = height * (uint32_t)std::ceil(cellSize);
constexpr float response_coef = 1.0f;



class Circle 
{
public:
	sf::Vector2f pos;
	sf::Vector2i gridPos;
	sf::Vector2f a = { 0,0 };
	sf::Vector2f oldPos;

	Circle(sf::Vector2f Pos, sf::Vector2f v)
		:
		pos(Pos),
		oldPos(Pos - v) {};

	void updatePos(float dt) {
		v = pos - oldPos;
		oldPos = pos;
		pos += v + a * dt * dt;
		a = { 0,0 };
	}

	void setVelocity(sf::Vector2f v) {
		oldPos = pos - v;
	}

private:
	sf::Vector2f v;
};








#if 0
class OldGrid
{
public:
	uint32_t* data;
	uint32_t* lengthData;
	uint32_t width, height, depth;

	Grid(uint32_t* data_, uint32_t* lengthData_, uint32_t width_, uint32_t height_, uint32_t depth_) :
		data(data_),
		lengthData(lengthData_),
		width(width_),
		height(height_),
		depth(depth_)
	{};
};
#endif


class Grid
{
public:
	Grid(uint32_t width, uint32_t height, uint32_t depth, std::vector<Circle>&circleRef ) : width_(width), height_(height), depth_(depth), circles(circleRef)
	{
		// grid allocation (3d array)
		// when accessing grid,  we will do with  grid[width][height][depth]
		grid = new uint32_t * *[width];
		for (uint32_t i = 0; i < width; i++)
		{
			grid[i] = new uint32_t * [height];

			for (uint32_t j = 0; j < height; j++)
			{
				grid[i][j] = new uint32_t[depth];
			}
		}




		// gridL allocation (2d array) (TODO)

		gridL = new uint32_t * [width];
		for (uint32_t i = 0; i < width; i++)
		{
			gridL[i] = new uint32_t[height];
		}


	}

	~Grid()
	{

		// gridL 
		for (uint32_t i = 0; i < width; i++)
		{			
			for (uint32_t j = 0; j < height; j++)
			{
				delete grid[i][j]; // Delete the array of depth for each dimention
			}
			delete grid[i]; // Delete the array of height for each dimention
		}
		delete grid; // Delete the array of width


		// gridL free

		for (uint32_t i = 0; i < width; i++)
		{
			delete gridL[i];
		}
		delete gridL;

	}


	void fillGrid() {
		for (uint32_t i = 0; i < width; i++) {
			for (uint32_t j = 0; j < height; j++) {
				gridL[i][j] = 0;
			}
		}
		int x;
		int y;
		for (uint32_t i = 0; i < circles.size(); i++) {
			x = static_cast<uint32_t> (std::floor(circles[i].pos.x / cellSize));
			y = static_cast<uint32_t> (std::floor(circles[i].pos.y / cellSize));
			if (gridL[x][y] < depth - 1) {
				grid[x][y][gridL[x][y]] = i;
				gridL[x][y]++;
			}
			circles[i].gridPos = { x, y };
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




private:


	void collide(uint32_t v1[], uint32_t v2[], uint32_t v1l, uint32_t v2l) {
		if (v2l == 0) {
			return;
		}
		else {
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


	uint32_t ***grid;
	uint32_t **gridL;
	std::vector<Circle> &circles;
	uint32_t width_;
	uint32_t height_;
	uint32_t depth_;

};


std::vector<Circle> circles;
uint32_t grid[width][height][depth];
uint32_t gridL[width][height];
Grid gridObject(width, height, depth, circles);


void testGrid()
{
	gridObject.fillGrid();

	gridObject.searchGrid();
}




void fillGrid() {
	for (uint32_t i = 0; i < width; i++) {
		for (uint32_t j = 0; j < height; j++) {
			gridL[i][j] = 0;
		}
	}
	int x;
	int y;
	for (uint32_t i = 0; i < circles.size(); i++) {
		x = static_cast<uint32_t> (std::floor(circles[i].pos.x / cellSize));
		y = static_cast<uint32_t> (std::floor(circles[i].pos.y / cellSize));
		if (gridL[x][y] < depth - 1) {
			grid[x][y][gridL[x][y]] = i;
			gridL[x][y]++;
		}
		circles[i].gridPos = { x, y };
	}
}



void collide(uint32_t v1[], uint32_t v2[], uint32_t v1l, uint32_t v2l) {
	if (v2l == 0) {
		return;
	}
	else {
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



class System {

	sf::Vector2f g = { 0,1000 };
	float dt;
	uint32_t subSteps;

	void addGravity(sf::Vector2f g) {
		for (uint32_t i = 0; i < circles.size(); i++) {
			circles[i].a = g;
		}
	}

	void boundingBox() {
		for (int i = 0; i < circles.size(); i++) {
			if (circles[i].pos.x <= cellSize * 2.1f) {
				circles[i].pos.x = cellSize * 2.1f;
			}
			if (circles[i].pos.x >= width * cellSize - 2.1f * cellSize) {
				circles[i].pos.x = width * cellSize - 2.1f * cellSize;
			}
			if (circles[i].pos.y <= cellSize * 2.1f) {
				circles[i].pos.y = cellSize * 2.1f;
			}
			if (circles[i].pos.y >= height * cellSize - 2.1f * cellSize) {
				circles[i].pos.y = height * cellSize - 2.1f * cellSize;
			}
		}
	}

	void update() {
		// apply gravity
		addGravity(g);

		// update positions
		for (uint32_t i = 0; i < circles.size(); i++) {
			circles[i].updatePos(dt);
		}

		boundingBox();
	}

public:
	sf::VertexArray quad{ sf::Quads, 80000 };

	System(float dt_, uint32_t subSteps_) :
		dt(dt_),
		subSteps(subSteps_)
	{};

	void nextFrame() {
		for (uint32_t i = 0; i < subSteps; i++) {
			fillGrid();
			searchGrid();
			update();
		}
		makeQuads();
	}

	void makeQuads() {
		const size_t l = circles.size();
		const uint32_t size = 1024;
		//quad.resize(l * 4);
		for (uint32_t i = 0; i < l; i++) {
			const sf::Vector2f* pos = &circles[i].pos;
			const uint32_t idx = i << 2;
			quad[idx + 0].position = circles[i].pos + sf::Vector2f(-radius, -radius);
			quad[idx + 1].position = circles[i].pos + sf::Vector2f(radius, -radius);
			quad[idx + 2].position = circles[i].pos + sf::Vector2f(radius, radius);
			quad[idx + 3].position = circles[i].pos + sf::Vector2f(-radius, radius);
			quad[idx + 0].texCoords = sf::Vector2f(0.0f, 0.0f);
			quad[idx + 1].texCoords = sf::Vector2f(size, 0.0f);
			quad[idx + 2].texCoords = sf::Vector2f(size, size);
			quad[idx + 3].texCoords = sf::Vector2f(0.0f, size);
		}
	}
};


