
#ifndef POOLTHREAD_H
#define POOLTHREAD_H

#include <thread>
#include <linux/futex.h>
#include <syscall.h>
#include <cstdlib>
#include <unistd.h>
#include <memory>
#include "Mutex.h"

class ThreadPool;

int partialFutex(int *uaddr, int futex_op, int val);

class PoolThread : std::thread {
    friend class ThreadPool;
public:
    PoolThread(ThreadPool* threadPool);
    PoolThread(const PoolThread& orig) = delete;
    PoolThread& operator = (const PoolThread& orig) = delete;
    ~PoolThread();
private:
    bool runFlag = true;
    void threadLoop();
    void sleep();
    ThreadPool* const threadPool;
    int* const waitQueue;
    static Mutex mutex;
};

#endif /* POOLTHREAD_H */

