#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "PoolThread.h"
#include <atomic>
#include <vector>
#include <iostream>

class ThreadPool {
    friend class PoolThread;
public:
    ThreadPool(const int n);
    ThreadPool(const ThreadPool& orig) = delete;
    ThreadPool(ThreadPool&& orig) = default;
    ThreadPool& operator=(const ThreadPool& orig) = delete;
    ThreadPool& operator=(ThreadPool&& orig) = default;
    ~ThreadPool();
    void doWork(voidFunc f);
    void shutdown();
private:
    int waitQueueNr;
    void wakeOne();
    void wakeAll();
    std::vector<PoolThread> threads;
    std::vector<voidFunc> work;
};

#endif /* THREADPOOL_H */

