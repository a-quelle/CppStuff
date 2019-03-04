#include <iostream>
#include "ThreadPool.h"
#include "Mutex.h"

using namespace std;

Mutex PoolThread::mutex;

int partialFutex(int *uaddr, int futex_op, int val)
{
    return syscall(SYS_futex, uaddr, futex_op, val,
                   nullptr, nullptr, 0);
}

PoolThread::PoolThread(ThreadPool* threadPool) : threadPool(threadPool), waitQueue(&(threadPool->waitQueueNr)), 
        thread(&PoolThread::threadLoop, this) {}
PoolThread::PoolThread(PoolThread&& orig) : threadPool(orig.threadPool), waitQueue(orig.waitQueue){};

PoolThread::~PoolThread(){}

void PoolThread::threadLoop(){
    while(runFlag)
    {
        mutex.lock();
        if(threadPool->work.empty())
        {
            mutex.unlock();
            sleep();
        }else{
            void (*f)() = threadPool->work.back();
            threadPool->work.pop_back();
            mutex.unlock();
            f();
        }        
    }
}

void PoolThread::sleep(){
    partialFutex(waitQueue, FUTEX_WAIT, *waitQueue);
}



