#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <SFML/Window.hpp>
#include <ViewPort.hpp>
#include <Grid.hpp>
#include <iostream>
#include <System.hpp>


using namespace std::chrono_literals;
class Solver
{
public:
	Solver(System* system_, Grid* grid_, sf::RenderWindow* window_)
		:runThread(&Solver::run, this),
		system(system_),
		grid(grid_),
		window(window_)
	{
		
	}

	~Solver()
	{
		std::unique_lock<std::mutex> lk;
		runThread.join();
	}

	void run() {
		Timing timing;
		while (window->isOpen()) {
			system->updatePos();
			timing.tick();
			if (grid->size() < 100000 && !system->isPaused()) {
				float pos = static_cast<float>(grid->getWidth() * grid->getCellSize() / 2);

				for (uint32_t i = 0; i < 100; i++) {
					Circle c({ pos, 20 }, { 0.3, 1 }, sf::Color::White);
					system->addElement(c);
					pos += grid->getCellSize();
				}
			}
		}
	}

private:
	std::thread runThread;
	System* system;
	Grid* grid;
	sf::RenderWindow* window;
};
