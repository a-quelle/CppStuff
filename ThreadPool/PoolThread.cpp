#include <iostream>
#include "ThreadPool.h"
#include "Mutex.h"

using namespace std;

Mutex PoolThread::mutex;
extern Mutex print;
PoolThread::PoolThread(ThreadPool* threadPool) : threadPool(threadPool),
        thread(&PoolThread::threadLoop, this) {}

PoolThread::~PoolThread(){}

void PoolThread::threadLoop(){
    print.lock();
    std::cout << "starting thread " << std::this_thread::get_id() << std::endl;    
    print.unlock();
    while(runFlag)
    {
        threadPool->semaphore.wait();
        mutex.lock();
        void (*f)() = threadPool->work.back();
        threadPool->work.pop_back();
        mutex.unlock();
        f();
    }
}



