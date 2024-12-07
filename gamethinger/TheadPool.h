#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
//#include <deque>
#include <future>
//#include <windows.h>//for sleep on windows

//#include <iostream>

class ThreadPool
{
private:
	static ThreadPool* singleton;
public:
	ThreadPool(int threadsCount = 2);
	void StopThreads();
	int GetThreadCount();

	//for lamdas or functions
	template <typename F, typename... Args>
	auto AddTask(F f, Args... args);

	//for class functions
	template <typename F, typename... Args>
	auto AddTask(F&& f, F& instance, Args... args);

	static ThreadPool* GetInstance(int threads = 1);

private:

	class BaseTask {
	public:
		virtual ~BaseTask() {};
		virtual void Run() {};
	};

	bool threadsRunning = false;
	std::vector<std::thread> threadPool;

	bool useQueueWait = false;
	std::condition_variable queueCV;
	std::mutex queueLock;
	std::queue<BaseTask*> tasksQueue;
	//std::queue<std::function<void()>> tasksQueue;

	template <typename F, typename... Args>
	class Task : public BaseTask {
	private:
		std::packaged_task<std::invoke_result_t<F, Args...>()> task;
	public:
		Task(F f, Args... args) : task(std::bind(f, args...))
		{}

		Task(F&& f, F& instance, Args... args) : task(std::bind(f, instance, args...))
		{}

		~Task() {
			//delete task;
		}

		std::future<std::invoke_result_t<F, Args...>> GetFuture()
		{
			return task.get_future();
		}

		void Run()
		{
			task();
		}
	};

	void DoWork(int threadID);
};

//for lamdas or functions
template <typename F, typename... Args>
auto ThreadPool::AddTask(F f, Args... args)
{
	//std::packaged_task<std::invoke_result_t<F, Args...>()> task_pack(std::bind(f, args...));
	//std::future<std::invoke_result_t<F, Args...>> future = task_pack.get_future();

	//task();

	//std::packaged_task<F(Args...)> task_pack(std::bind_front(f, args...));
	//std::future<F> future = task_pack.get_future();

	//Task t(std::forward(std::move(task_pack)));

	//task_pack();
	//tasksQueue.emplace([&]() mutable { return task_pack(); });

	Task<F, Args...>* t = new Task(f, args...);
	std::future<std::invoke_result_t<F, Args...>> future = t->GetFuture();
	
	std::unique_lock lock(queueLock);

	tasksQueue.emplace(t);

	useQueueWait = true;
	lock.unlock();
	queueCV.notify_all();

	return future;
}

//for class functions
template <typename F, typename... Args>
auto ThreadPool::AddTask(F&& f, F& instance, Args... args)
{
	Task<F, Args...>* t = new Task(f, instance, args...);
	std::future<std::invoke_result_t<F, Args...>> future = t->GetFuture();

	std::unique_lock lock(queueLock);

	tasksQueue.emplace(t);

	//useQueueWait = true;
	lock.unlock();
	queueCV.notify_one();

	return future;
}

//for class functions
//template <typename F, typename... Args>
//auto ThreadPool::AddTask(auto(F::* f)(Args...), F& instance, Args... args)
//{
//	std::packaged_task<decltype(std::declval<F&>()[instance{}]) > task_pack(std::bind(f, instance, args...));	
//	
//	//std::packaged_task<decltype(std::declval<F&>()[f{}]) > task_pack(std::bind(f, instance, args...));
//
//	//std::packaged_task<std::invoke_result_t<F, Args...>()> task_pack(std::bind(&(F::*)(instance.*f), instance, args...));
//	//std::future<std::invoke_result_t<F, Args...>> future = task_pack.get_future();
//
//	//return future;
//
//	//return future;// (instance.*f)(args...);
//	return 1;
//}

