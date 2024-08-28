#include "ThreadPool.hpp"
#include "utils.hpp"

ThreadPool::ThreadPool(int size) : stop(false)
{
    for(int i = 0; i < size; ++i) {
        threads.emplace_back(std::thread(std::bind(&ThreadPool::run, this)));
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lk(tasks_mtx);
        stop = false;
    }
    cv.notify_all();
    for(std::thread & th : threads) {
        if(th.joinable()) th.join();
    }
}


void ThreadPool::addTask(std::function<void()> func)
{
    INFO("add task");
    {
        std::unique_lock<std::mutex> lk(tasks_mtx);
        if(stop) {
            throw std::runtime_error("threadpool have stopped!");
            return;
        }

        tasks.emplace(func);
    }
    cv.notify_one();
}

void ThreadPool::run()
{
    while (stop != true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(tasks_mtx);
            while (tasks.empty() == true)
            {
                cv.wait(lock);
            }

            task = tasks.front();
            tasks.pop();
        }
        INFO("exec task");
        task();
    }
}
