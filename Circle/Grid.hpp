#pragma once
#include <Constraints.hpp>
#include <memory>
#include <Examples.hpp>
#include <functional>

//#define MULTIPROC 1
template <class T>
class Grid
{
public:
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
		gridProcessingFunction = [&](uint32_t startIndex, uint32_t endIndex) {
			searchGridProcessing(startIndex, endIndex);
			};
		multiProcess_uPtr = std::make_unique<MultiThreadedProcessing>(1, gridProcessingFunction);
		multiProcess_uPtr->setNumElements(width);
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
		int x, y;
		for (uint32_t i = 0; i < circles.size(); i++) {
			x = static_cast<uint32_t> (std::floor(circles[i].pos.x / cellSize));
			y = static_cast<uint32_t> (std::floor(circles[i].pos.y / cellSize));
			if (gridL[x][y] < depth - 1) {
				grid[x][y][gridL[x][y]] = i;
				gridL[x][y]++;
			}
		}
	}

	void searchGrid(uint32_t startIdx, uint32_t endIdx) {
#ifdef MULTIPROC
		multiProcess_uPtr->setNumElements(endIdx - startIdx);
		multiProcess_uPtr->processAll();
#else
		searchGridProcessing(startIdx, endIdx);
#endif
	}


	void searchGridProcessing(uint32_t startIdx, uint32_t endIdx) {
		//const uint32_t x = width - 2;
		const uint32_t y = height - 2;
		for (uint32_t i = endIdx; i > startIdx; i--) {
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

	void makeVAs() {
		const size_t l = size();
		const uint32_t size = 1024;
		for (uint32_t i = 0; i < l; i++) {
			const sf::Vector2f* pos = &circles[i].pos;
			const uint32_t idx = i << 2;
			objectVA[idx + 0].position = circles[i].pos + sf::Vector2f(-radius, -radius);
			objectVA[idx + 1].position = circles[i].pos + sf::Vector2f(radius, -radius);
			objectVA[idx + 2].position = circles[i].pos + sf::Vector2f(radius, radius);
			objectVA[idx + 3].position = circles[i].pos + sf::Vector2f(-radius, radius);
			objectVA[idx + 0].texCoords = sf::Vector2f(0.0f, 0.0f);
			objectVA[idx + 1].texCoords = sf::Vector2f(size, 0.0f);
			objectVA[idx + 2].texCoords = sf::Vector2f(size, size);
			objectVA[idx + 3].texCoords = sf::Vector2f(0.0f, size);
		}
	}

	void handleConstraints() {
		for (uint32_t i = 0; i < links.size(); i++) {
			links[i].update(circles);
		}
	}

	void addElementToGrid(T& element)
	{
		circles.emplace_back(element);
	}

	void updateElPos(float dt) {
		for (uint32_t i = 0; i < circles.size(); i++) {
			circles[i].updatePos(dt);
		}
	}

	void setGravity(sf::Vector2f g) {
		// Lock
		for (uint32_t i = 0; i < circles.size(); i++) {
			circles[i].set_a(g);
		}
		// Unlock
	}

	void drawElements(sf::RenderWindow& window, sf::RenderStates& states) {
		window.draw(objectVA, states);
	}

	uint32_t getWidth() {
		return width;
	}
	uint32_t getHeight() {
		return height;
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
							ob1->pos += delta * (1 + ob2->holdPos) * !ob1->holdPos * dir;
							ob2->pos += -delta * (1 + ob1->holdPos) * !ob2->holdPos * dir;
						}
					}
				}
			}
		}
	}
	const float response_coef = 1.0f;
	const float radius;
	const float cellSize;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	uint32_t*** grid;
	uint32_t** gridL;
	std::vector<T> circles;
	std::vector <Link> links;
	sf::VertexArray objectVA{ sf::Quads, 160000 };

	std::function<void(uint32_t startIndex, uint32_t endIndex)> gridProcessingFunction;
	std::unique_ptr<MultiThreadedProcessing> multiProcess_uPtr;

};