#pragma once
#include <Grid.hpp>

class System {
public:
	System(float dt_, uint32_t subSteps_, Grid<Circle>& grid_, sf::RenderWindow& window_, sf::RenderStates& states_) :
		dt(dt_),
		subSteps(subSteps_),
		grid(grid_),
		window(window_),
		states(states_)
	{};

	void updatePos() {
		for (uint32_t i = 0; i < subSteps; i++) {
			grid.fillGrid();
			grid.searchGrid();
			update();
		}
		grid.makeVAs();
	}

	void drawFrame() {
		grid.drawElements(window, states);
	}

	/*void makeRigidBody(float x, float y, uint32_t width, uint32_t height, float rigigity) {
		float diaL = sqrt(grid.cellSize * grid.cellSize + grid.cellSize * grid.cellSize);
		for (uint32_t i = 0; i < width; i++) {
			for (uint32_t j = 0; j < height; j++) {
				Circle c({ { x + i * grid.cellSize, y + j * grid.cellSize }, { 0,0 } });
				grid.addElementToGrid(c);
				if (j != 0) {
					links.push_back({ grid.size() - 1, grid.size() - 2, grid.cellSize, rigigity});
				}

				if (i != 0) {
					links.push_back({ grid.size() - 1, grid.size() - (height + 1) , grid.cellSize, rigigity });

					if (j < height - 1) {
						links.push_back({ grid.size() - 1, grid.size() - height , diaL, rigigity });
					}

					if (j != 0) {
						links.push_back({ grid.size() - 1, grid.size() - (height + 2) , diaL, rigigity });
					}
				}
			}
		}
	}

	void makeRope(float x, float y, uint32_t length, float rigigity) {
		Circle c({ { x, y }, { 0,0 } });
		grid.addElementToGrid(c);
		pins.push_back({ grid.size() - 1, { x, y } });
		for (uint32_t i = 1; i < length; i++) {
			Circle c({ { x, y + i * grid.cellSize }, { 0,0 } });
			grid.addElementToGrid(c);
			links.push_back({ grid.size() - 1, grid.size() - 2, grid.cellSize, rigigity });
		}
	}*/


private:
	sf::Vector2f g = { 0, 1000 };
	float dt;
	uint32_t subSteps;
	Grid<Circle>& grid;
	sf::RenderWindow& window;
	sf::RenderStates& states;

	void addGravity(sf::Vector2f g) {
		grid.setGravity(g);
	}

	void update() {
		// apply gravity
		addGravity(g);
		grid.handleConstraints();

		// update positions
		grid.updateElPos(dt);
		grid.boundingBox();
	}
};