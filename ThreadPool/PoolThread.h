
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

class PoolThread : std::thread {
    friend class ThreadPool;
public:
    PoolThread(ThreadPool* threadPool);
    PoolThread(const PoolThread& orig) = delete;
    PoolThread(PoolThread&& orig) = default;
    PoolThread& operator = (const PoolThread& orig) = delete;
    PoolThread& operator = (PoolThread&& orig) = default;
    ~PoolThread();
private:
    bool runFlag = true;
    void threadLoop();
    ThreadPool* const threadPool;
    static Mutex mutex;
};

#endif /* POOLTHREAD_H */

