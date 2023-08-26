#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

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


class Link
{	
public:
	Link(uint64_t idx1_, uint64_t idx2_, float dist_, float rigigity_) :
		idx1(idx1_), idx2(idx2_), dist(dist_), rigigity(rigigity_)
	{};

	Link(uint64_t idx1_, uint64_t idx2_, sf::Vector2f pos1, sf::Vector2f pos2, float rigigity_) :
		idx1(idx1_), idx2(idx2_), rigigity(rigigity_)
	{
		const sf::Vector2f axis = pos1 - pos2;
		dist = sqrt(axis.x * axis.x + axis.y * axis.y);
	};

	~Link() = default;

	void update(std::vector<Circle>& circles) {
		Circle& ob1 = circles[idx1];
		Circle& ob2 = circles[idx2];
		const sf::Vector2f axis = ob1.pos - ob2.pos;
		const float d = sqrt(axis.x * axis.x + axis.y * axis.y);
		const sf::Vector2f dir = axis / dist;
		const float delta = dist - d;
		ob1.pos += 0.5f * delta * dir * rigigity;
		ob2.pos -= 0.5f * delta * dir * rigigity;
	}
private:
	float dist;
	const float rigigity;
	const uint64_t idx1;
	const uint64_t idx2;
};

class Pin
{
public:

	Pin(uint64_t idx_, sf::Vector2f pos_):
		idx(idx_), pos(pos_)
	{};

	void update(std::vector<Circle>& circles) {
		Circle& ob = circles[idx];
		ob.pos = pos;
	}

private: 
	sf::Vector2f pos;
	uint64_t idx;
};

template <class T>
class Grid
{
public:

	const float radius;
	const float cellSize;
	std::vector<T> circles;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
			
	Grid(uint32_t width_, uint32_t height_, uint32_t depth_, float cellSize_) 
		: width(width_), height(height_), depth(depth_), cellSize(cellSize_), radius(cellSize_ / 2.0f)
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

		gridL = new uint32_t * [width];
		for (uint32_t i = 0; i < width; i++)
		{
			gridL[i] = new uint32_t[height];
		}

	}

	~Grid()
	{

		// grid free
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


	size_t size()
	{
		return circles.size();
	}

	void fillGrid() {
		for (uint32_t i = 0; i < width; i++) {
			for (uint32_t j = 0; j < height; j++) {
				gridL[i][j] = 0;
			}
		}
		int x,y;
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

	void addElementToGrid(T &element)
	{
		circles.emplace_back(element);
	}

	void setGravity(sf::Vector2f g) {
		// Lock
		for (uint32_t i = 0; i < circles.size(); i++) {
			circles[i].a = g;
		}
		// Unlock
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
	const float response_coef = 1.0f;
	uint32_t ***grid;
	uint32_t **gridL;

};



class System {
public:
	System(float dt_, uint32_t subSteps_, Grid<Circle>& grid_, sf::RenderWindow& window_, sf::RenderStates& states_, std::vector<Link>& links_) :
		dt(dt_),
		subSteps(subSteps_),
		grid(grid_),
		window(window_),
		states(states_),
		links(links_)
	{};

	void updatePos() {
		for (uint32_t i = 0; i < subSteps; i++) {
			grid.fillGrid();
			grid.searchGrid();
			update();
		}
		makeVAs();
	}

	void drawFrame() {
		window.draw(objectVA, states);
	}

	void makeRigidBody(float x, float y, uint32_t width, uint32_t height, float rigigity) {
		float diaL = sqrt(grid.cellSize * grid.cellSize + grid.cellSize * grid.cellSize);
		for (uint32_t i = 0; i < width; i++) {
			for (uint32_t j = 0; j < height; j++) {
				Circle c({ { x + i * grid.cellSize, y + j * grid.cellSize }, { 0,0 } });
				grid.addElementToGrid(c);
				if (j != 0) {
					links.push_back({ grid.size() - 1, grid.circles.size() - 2, grid.cellSize, rigigity});
				}

				if (i != 0) {
					links.push_back({ grid.circles.size() - 1, grid.circles.size() - (height + 1) , grid.cellSize, rigigity });

					if (j < height - 1) {
						links.push_back({ grid.circles.size() - 1, grid.circles.size() - height , diaL, rigigity });
					}

					if (j != 0) {
						links.push_back({ grid.circles.size() - 1, grid.circles.size() - (height + 2) , diaL, rigigity });
					}
				}
			}
		}
	}

	void makeRope(float x, float y, uint32_t length, float rigigity) {
		grid.circles.push_back({ { x, y }, { 0,0 } });
		pins.push_back({ grid.circles.size() - 1, { x, y } });
		for (uint32_t i = 1; i < length; i++) {
			grid.circles.push_back({ { x, y + i * grid.cellSize }, { 0,0 } });
			links.push_back({ grid.circles.size() - 1, grid.circles.size() - 2, grid.cellSize, rigigity });
		}
	}


private:
	sf::Vector2f g = { 0, 1000 };
	sf::VertexArray objectVA{ sf::Quads, 80000 };
	sf::VertexArray linkVA{ sf::Lines, 10000 };
	float dt;
	uint32_t subSteps;
	Grid<Circle>& grid;
	sf::RenderWindow& window;
	sf::RenderStates& states;
	std::vector <Link> links;
	std::vector <Pin> pins;

	void addGravity(sf::Vector2f g) {
		grid.setGravity(g);
	}

	void handleConstraints() {
		for (uint32_t i = 0; i < links.size(); i++) {
			links[i].update(grid.circles);
		}

		for (uint32_t i = 0; i < pins.size(); i++) {
			pins[i].update(grid.circles);
		}
	}

	void update() {
		// apply gravity
		addGravity(g);
		handleConstraints();

		// update positions
		for (uint32_t i = 0; i < grid.circles.size(); i++) {
			grid.circles[i].updatePos(dt);
		}
		grid.boundingBox();
	}

	void makeVAs() {
		const size_t l = grid.circles.size();
		const uint32_t size = 1024;
		for (uint32_t i = 0; i < l; i++) {
			const sf::Vector2f* pos = &grid.circles[i].pos;
			const uint32_t idx = i << 2;
			const float& radius = grid.radius;
			objectVA[idx + 0].position = grid.circles[i].pos + sf::Vector2f(-radius, -radius);
			objectVA[idx + 1].position = grid.circles[i].pos + sf::Vector2f(radius, -radius);
			objectVA[idx + 2].position = grid.circles[i].pos + sf::Vector2f(radius, radius);
			objectVA[idx + 3].position = grid.circles[i].pos + sf::Vector2f(-radius, radius);
			objectVA[idx + 0].texCoords = sf::Vector2f(0.0f, 0.0f);
			objectVA[idx + 1].texCoords = sf::Vector2f(size, 0.0f);
			objectVA[idx + 2].texCoords = sf::Vector2f(size, size);
			objectVA[idx + 3].texCoords = sf::Vector2f(0.0f, size);
		}
	}
};