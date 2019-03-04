#include "ThreadPool.h"
#include <atomic>
#include <algorithm>
#include <limits>

ThreadPool::ThreadPool(const int n) {
    waitQueueNr = 1337;
    threads.reserve(n);
    for (int i = 0; i < n; i++){
        threads.emplace_back(this);
    }
}

void ThreadPool::doWork(void(*f)()){
    PoolThread::mutex.lock();
    work.push_back(f);
    PoolThread::mutex.unlock();
    wakeOne();
}

void ThreadPool::shutdown() {
    std::for_each(threads.begin(), threads.end(), [](PoolThread& thread){ thread.runFlag = false;});
    wakeAll();
    std::for_each(threads.begin(), threads.end(), [](PoolThread& thread){ thread.join();});
}

ThreadPool::~ThreadPool() {
    std::for_each(threads.begin(), threads.end(),[](PoolThread& thread){ thread.runFlag = false;});
}

void ThreadPool::wakeOne(){
    partialFutex(&waitQueueNr, FUTEX_WAKE, 1);
}

void ThreadPool::wakeAll(){
    partialFutex(&waitQueueNr, FUTEX_WAKE, std::numeric_limits<int>::max());
}