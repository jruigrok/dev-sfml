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
	Solver(System* system_, sf::RenderWindow* window_)
		:runThread(&Solver::run, this),
		system(system_),
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
		}
	}

private:
	std::thread runThread;
	System* system;
	sf::RenderWindow* window;
};
