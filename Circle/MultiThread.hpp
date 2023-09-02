#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <memory>
#include <iostream>
#include <array>
#include <exception>
#include <sstream>
#include <functional>

class ThreadControl;


class MultiThreadedProcessing
{
public:
	MultiThreadedProcessing(uint32_t numThreads, std::function<void(uint32_t startIndex, uint32_t endIndex)> function);
	~MultiThreadedProcessing();	
	void processAll();	
	bool setNumElements(uint32_t numElements);
private:
	void threadFunction(ThreadControl& data);
	void setElementsPerThread();
	//std::vector<uint32_t> elementsCount_;
	std::unique_ptr<ThreadControl[]> threadControl_;
	std::vector<std::thread> threads_;
	uint32_t numElements_ = 1;
	uint32_t numThreads_ = 1;
	std::mutex interfaceMutex_;
	std::function<void(uint32_t startIndex, uint32_t endIndex)> processingFunction_;
};
