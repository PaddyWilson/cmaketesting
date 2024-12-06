#include "TheadPool.h"

ThreadPool* ThreadPool::singleton = nullptr;

ThreadPool::ThreadPool(int threadsCount)
{
	threadsRunning = true;
	//create pool of threads;
	for (size_t i = 0; i < threadsCount; i++) {
		threadPool.push_back(std::thread([&](int id) { 
			DoWork(id); 
			}, i));
	}
}

void ThreadPool::StopThreads() {
	threadsRunning = false;
	queueCV.notify_all();
	for (size_t i = 0; i < threadPool.size(); i++)
		threadPool[i].join();
};

int ThreadPool::GetThreadCount()
{
	return threadPool.size();
}

ThreadPool* ThreadPool::GetInstance(int threads)
{
	if (singleton == nullptr)
		singleton = new ThreadPool(threads);
	return singleton;
}
#include <iostream>
void ThreadPool::DoWork(int threadID)
{
	while (threadsRunning)
	{
		BaseTask* task;
		{
			std::unique_lock lock(queueLock);
			queueCV.wait(lock, [&]() {
				return !threadsRunning || !tasksQueue.empty();
				});

			if (!threadsRunning)
			{
				lock.unlock();
				return;
			}

			//do jobs
			//if (tasksQueue.empty())
			//{
			//	lock.unlock();
			//	//std::this_thread::sleep_for(std::chrono::microseconds(1));
			//	continue;
			//}

			task = tasksQueue.front();
			tasksQueue.pop();
			lock.unlock();
			//std::cout << "Got Task Thread:" << threadID << std::endl;
		}
		task->Run();
		delete task;
	}
}