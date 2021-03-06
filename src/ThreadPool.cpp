#include "ThreadPool.h"
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------
namespace
{
//----------------------------------------------------------------------------------------------------------------------
static const constexpr unsigned int defaultNumberOfThreads = 2;
#ifdef N_DEBUG
static const constexpr char* logPrefix = "[ThreadPool]: ";
#endif
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
namespace MT
{
//----------------------------------------------------------------------------------------------------------------------
ThreadPool::ThreadPool(unsigned int numberOfThreads)
    :destroyFlag_(false)
{
    numberOfThreads = std::max(numberOfThreads, defaultNumberOfThreads);

#ifdef N_DEBUG
    std::cerr << logPrefix << "Creating " << numberOfThreads << " threads\n";
#endif

    for(unsigned int i = 0; i < numberOfThreads; ++i)
    {
        std::thread th([this, i]
        {
            for(;;)
            {
                Task currentTask;
                std::unique_lock<std::mutex> locker(threadMutex_);
                waitCondition_.wait(locker, [this, &currentTask] { return destroyFlag_ || tryToGetTask(currentTask); });

                if(destroyFlag_.load())
                {
#ifdef N_DEBUG
                    std::cerr << logPrefix << "Returning from thread #" << i "\n";
#endif
                    return;
                }

                currentTask();
            }
        });
        threads_.push_back(std::move(th));
    }
}
//----------------------------------------------------------------------------------------------------------------------
ThreadPool::~ThreadPool()
{
#ifdef N_DEBUG
    std::cerr << logPrefix << "Destroying thread pool\n";
#endif
    destroyFlag_.store(true);
    for (auto& th : threads_)
    {
        if(th.joinable())
            th.join();
    }
}
//----------------------------------------------------------------------------------------------------------------------
bool ThreadPool::tryToGetTask(Task& t)
{
    std::lock_guard<std::mutex> lock(queueMutex_);
    if(tasks_.empty()) return false;

    t.swap(tasks_.front());
    tasks_.pop();

    return true;
}
//----------------------------------------------------------------------------------------------------------------------
}
