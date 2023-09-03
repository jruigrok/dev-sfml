#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
using namespace std;

using namespace std::chrono_literals;
class Timing
{
public:
	Timing():runThread(&Timing::threadFunction, this)
	{}

	~Timing()
	{
		std::unique_lock<std::mutex> lk;
		exit_ = true;
		cv.notify_all();
		runThread.join();

	}


	void tick()
	{
		std::unique_lock<std::mutex> lk(mutex);
		tickCount_++;
	}

	std::string& getCountString()
	{		
		outString = std::string("Frame Count:") + std::to_string(tickCountLast_) + std::string("\n");
		return outString;
	}

	void threadFunction()
	{
		while (true)
		{
			std::this_thread::sleep_for(1000ms);
			{
				std::unique_lock<std::mutex> lk;
				if (exit_)
				{
					return;
				}
				tickCountLast_ = tickCount_;
				cout << "fps: " << tickCount_ << endl;
				tickCount_ = 0;
			}
		}
	}

private:
	uint32_t tickCount_=0;
	uint32_t tickCountLast_ = 0;
	std::mutex mutex;
	bool exit_ = false;
	std::condition_variable cv;
	std::thread runThread;
	std::string outString;

};
