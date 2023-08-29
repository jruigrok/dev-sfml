#include <Examples.hpp>

class ThreadControl {
public:

	ThreadControl()
	{
		threadId = ++currentId;
	}
	std::condition_variable threadCv;
	std::condition_variable threadCompletedCv; // for completed and waitForCompletion
	std::mutex threadMutex;
	bool start = false;
	bool completed = false;
	bool forceEnd = false;
	uint32_t startIndex = 0;
	uint32_t endIndex = 0;
	uint32_t threadId;
	static uint32_t currentId;
	bool startProcessing()
	{
		std::unique_lock<std::mutex> lk(threadMutex);
		if ((startIndex > endIndex) || (startIndex >= endIndex))
		{
			std::cerr << "Invalid start argument: start: " << std::to_string(startIndex) << " end:" << std::to_string(endIndex) << std::endl;
			return false;
		}
		completed = false;
		start = true;
		threadCv.notify_one();
	}

	bool waitForCompletion()
	{
		std::unique_lock<std::mutex> lk(threadMutex);
		threadCompletedCv.wait(lk, [&] {return (completed == true || forceEnd == true); });
		return completed == false;
	}
};

// This is neccessary for a static component to be defined by the linker
uint32_t ThreadControl::currentId;

MultiThreadedProcessing::MultiThreadedProcessing(uint32_t numThreads, std::function<void(uint32_t startIndex, uint32_t endIndex)> function) :numElements_(0), numThreads_(numThreads), processingFunction_(function)
{
	// To start, there will be 0 elements until setNumElements is called.
	elementsCount_.resize(numElements_);
	threadControl_ = std::unique_ptr<ThreadControl[]>(new ThreadControl[numThreads_]);
	setElementsPerThread();
	
	for (uint32_t i = 0; i < numThreads; i++)
	{
		// Spawn threads and pass each thread their own data structure
		// Note std::ref here is necessary to pass the reference to the threadFunction
		threads_.emplace_back(&MultiThreadedProcessing::threadFunction, this, std::ref(threadControl_[i]));
	}
}

MultiThreadedProcessing::~MultiThreadedProcessing()
{
	for (uint32_t i = 0; i < numThreads_; i++)
	{
		{
			std::unique_lock<std::mutex> lk(threadControl_[i].threadMutex);
			threadControl_[i].forceEnd = true;
		}
		threadControl_[i].threadCv.notify_one();
		if (threads_[i].joinable())
		{
			threads_.at(i).join();
		}
	}
}

bool  MultiThreadedProcessing::setNumElements(uint32_t numElements)
{
	if (numElements == 0)
	{
		return false;
	}
	if (numElements < numThreads_)
	{
		std::cerr << "Invalid configuration, num Elements too small for numThreads" << std::endl;
		return false;
	}
	std::unique_lock<std::mutex> lk(interfaceMutex_);
	numElements_ = numElements;
	// elementsCount_ is for testing only, but it needs to be the same size of numElements_ always
	elementsCount_.resize(numElements_);
	setElementsPerThread();
	return true;
}

void MultiThreadedProcessing::setElementsPerThread()
{	
	if (numThreads_ == 1)
	{
		// Corner Case
		threadControl_[0].startIndex = 0;
		threadControl_[0].endIndex = numElements_ - 1;
	}
	else if ((numElements_ % numThreads_) == 0)
	{
		// Perfect division
		uint32_t startIndex = 0;
		uint32_t numIndexPerThread = numElements_ / numThreads_;

		for (uint32_t i = 0; i < numThreads_; i++)
		{
			threadControl_[i].startIndex = startIndex;
			threadControl_[i].endIndex = startIndex + numIndexPerThread - 1;
			startIndex = threadControl_[i].endIndex + 1;
			//std::cout << "thread index[" << std::to_string(i) << "]  start:" << std::to_string(threadControl_[i].startIndex) << " end: " << std::to_string(threadControl_[i].endIndex) << std::endl;
		}
	}
	else
	{
		uint32_t startIndex = 0;
		uint32_t numIndexPerThread = numElements_ / (numThreads_);

		// Use the last thread for the remainder, could be further optimize by splitting the remainder among multiple threads
		for (uint32_t i = 0; i < numThreads_; i++)
		{
			threadControl_[i].startIndex = startIndex;
			threadControl_[i].endIndex = startIndex + numIndexPerThread - 1;
			startIndex = threadControl_[i].endIndex + 1;
			//std::cout << "thread index[" << std::to_string(i) << "]  start:" << std::to_string(threadControl_[i].startIndex) << " end: " << std::to_string(threadControl_[i].endIndex) << std::endl;
		}

		// Last one, add remainder to last thread			
		threadControl_[numThreads_ - 1].endIndex += numElements_ % (numThreads_);
		//std::cout << "thread index[" << std::to_string(numThreads_ - 1) << "]  start:" << std::to_string(threadControl_[numThreads_ - 1].startIndex) << " end: " << std::to_string(threadControl_[numThreads_ - 1].endIndex) << std::endl;
	}
}


void MultiThreadedProcessing::processAll()
{
	std::unique_lock<std::mutex> lk(interfaceMutex_);
	if (numElements_ == 0)
	{
		return;
	}
	// This prevents updateNumElements from being called while processAll is running
	for (uint32_t i = 0; i < numThreads_; i++)
	{
		threadControl_[i].startProcessing();
	}

	for (uint32_t i = 0; i < numThreads_; i++)
	{
		threadControl_[i].waitForCompletion();
	}

}


void MultiThreadedProcessing::threadFunction(ThreadControl& data)
{
	// Note start and end Index are inclusive, meaning we process start to end, including the end index.
	std::stringstream ss;
	while (!data.forceEnd)
	{
		std::unique_lock<std::mutex> lk(data.threadMutex);
		data.threadCv.wait(lk, [&data] {return (data.start == true || data.forceEnd == true); });
		if (data.forceEnd == true)
		{
			return;
		}
		// Here we got the start signal, and we have the lock
		
		// Call the processing function callback with the range of indexes to process
		processingFunction_(data.startIndex, data.endIndex);

		// For debugging only
#if 0
		ss << "processing Thread[" << std::to_string(data.threadId) << "]: element:";
		for (auto i = data.startIndex; i <= data.endIndex; i++)
		{
			// Here you do what ever processing is needed on the specific elments 
			elementsCount_.at(i)++;
			ss << "[" << std::to_string(i) << "]:" << std::to_string(elementsCount_.at(i)) << " ";
		}
		ss << std::endl;
		std::cout << ss.str();
		ss.clear();
#endif
		data.completed = true;
		data.threadCompletedCv.notify_one();
		// Here we reset start, and at the data.threadCv.wait, we will release the lock, so the start function can be called again.
		data.start = false;
	}

}