#pragma once
#include <Grid.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <ViewPort.hpp>

class System {
public:
	System(uint32_t subSteps_, Grid& grid_, sf::RenderWindow& window_, ViewPort& viewPort_) :
		subSteps(subSteps_),
		grid(grid_),
		window(window_),
		viewPort(viewPort_)
	{};

	void updatePos() {
		for (uint32_t i = 0; i < subSteps; i++) {
			grid.updateGrid();
		}
		grid.makeEl_VAs();
	}

	void drawFrame() {
		grid.drawElements(window, viewPort.objectStates);
		grid.drawBoarder(window, viewPort.states);
	}

	void makeRigidBody(float x, float y, uint32_t width, uint32_t height, float rigigity) {
		float diaL = sqrt(grid.getCellSize() * grid.getCellSize() * 2.0f);
		for (uint32_t i = 0; i < width; i++) {
			for (uint32_t j = 0; j < height; j++) {
				Circle c({ { x + i * grid.getCellSize(), y + j * grid.getCellSize() }, { 0,0 } });
				grid.addElementToGrid(c);
				if (j != 0) {
					Link l = { grid.size() - 1, grid.size() - 2, grid.getCellSize(), rigigity };
					grid.addConstraintToGrid(l);
				}

				if (i != 0) {
					Link l = { grid.size() - 1, grid.size() - (height + 1) , grid.getCellSize(), rigigity };
					grid.addConstraintToGrid(l);

					if (j < height - 1) {
						Link l = { grid.size() - 1, grid.size() - height , diaL, rigigity };
						grid.addConstraintToGrid(l);
					}

					if (j != 0) {
						Link l = { grid.size() - 1, grid.size() - (height + 2) , diaL, rigigity };
						grid.addConstraintToGrid(l);
					}
				}
			}
		}
	}

	void makeRope(float x, float y, uint32_t length, float rigigity) {
		Circle c({ { x, y }, { 0,0 } });
		c.holdPos = 1;
		grid.addElementToGrid(c);
		for (uint32_t i = 1; i < length; i++) {
			Circle c({ { x, y + i * grid.getCellSize()}, {0,0}});
			grid.addElementToGrid(c);
			Link l({ grid.size() - 1, grid.size() - 2, grid.getCellSize(), rigigity });
			grid.addConstraintToGrid(l);
		}
	}

	void makeBoarder(uint32_t cellSize) {
		Circle c({ { cellSize / 2.0f, cellSize / 2.0f }, {0,0} });
		c.holdPos = true;
		for (uint32_t i = 0; i < grid.getWidth(); i++) {
			grid.addElementToGrid(c);
			c.pos.x += cellSize;
		}
		c.pos = { cellSize / 2.0f ,grid.getHeight() * cellSize - cellSize / 2.0f };
		for (uint32_t i = 0; i < grid.getWidth(); i++) {
			grid.addElementToGrid(c);
			c.pos.x += cellSize;
		}
		c.pos = { cellSize * 2.0f ,cellSize * 1.5f };

		for (uint32_t i = 0; i < grid.getHeight() - 2; i++) {
			grid.addElementToGrid(c);
			c.pos.y += cellSize;
		}
		c.pos = { grid.getWidth() * cellSize - cellSize * 2.0f ,cellSize * 1.5f };

		for (uint32_t i = 0; i < grid.getHeight() - 2; i++) {
			grid.addElementToGrid(c);
			c.pos.y += cellSize;
		}
	}

	void handleEvents(sf::Event& event) {
		if (event.type == sf::Event::MouseWheelMoved) {
			if (event.mouseWheel.delta < 0) {
				viewPort.zoomOnPoint(scrollZoomMag, { (float)event.mouseWheel.x, (float)event.mouseWheel.y });
			}
			else {
				viewPort.zoomOnPoint(1.0f / scrollZoomMag, { (float)event.mouseWheel.x, (float)event.mouseWheel.y });
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2f truePos = viewPort.getTruePos({ (float)event.mouseButton.x, (float)event.mouseButton.y });
			if (grid.inBoarder(truePos)) {
				anchorPos = truePos;
			}
			mouseDown = true;
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			mouseDown = false;
		}
		else if (event.type == sf::Event::MouseMoved && mouseDown) {

			sf::Vector2f truePos = viewPort.getTruePos({ (float)event.mouseMove.x, (float)event.mouseMove.y });

			if (grid.inBoarder(truePos)) {

				sf::Vector2f gridPos = (sf::Vector2f)grid.getGridPos(truePos);
				switch (mode) {
				case 0:
					viewPort.move((truePos - anchorPos) * viewPort.getScale());
					break;
				case 1:
					if (grid.getLength(grid.getGridPos(truePos)) == 0) {
						Circle c({ gridPos * grid.getCellSize(), {0,0} });
						c.holdPos = true;
						grid.addElementToGrid(c);
					}
					break;
				case 2:
					break;
				}
			}
		}
		else if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case 26:
				mode = 0;
				break;
			case 27:
				mode = 1;
				break;
			case 28:
				mode = 2;
				break;
			}
		}
	}

	

private:
	


	uint32_t subSteps;
	Grid& grid;
	sf::RenderWindow& window;
	ViewPort& viewPort;

	float scrollZoomMag = 0.8f;
	bool mouseDown = false;
	uint32_t mode = 0;
	sf::Vector2f anchorPos = { 0,0 };
};