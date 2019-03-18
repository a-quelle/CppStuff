#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "PoolThread.h"
#include <atomic>
#include <vector>
#include <iostream>

struct Semaphore{
    Semaphore(const uint n);
    Semaphore(const Semaphore& orig) = delete;
    Semaphore(Semaphore&& orig) = default;
    Semaphore& operator=(const Semaphore& orig) = delete;
    Semaphore& operator=(Semaphore&& orig) = default;
    void wait();
    void signal();   
    std::atomic_uint value;
};

class ThreadPool {
    friend class PoolThread;
public:
    ThreadPool(const int n);
    ThreadPool(const ThreadPool& orig) = delete;
    ThreadPool(ThreadPool&& orig) = default;
    ThreadPool& operator=(const ThreadPool& orig) = delete;
    ThreadPool& operator=(ThreadPool&& orig) = default;
    ~ThreadPool();
    void submitWork(voidFunc f);
private:
    Semaphore semaphore;
    std::vector<PoolThread> threads;
    std::vector<voidFunc> work;
};

#endif /* THREADPOOL_H */

