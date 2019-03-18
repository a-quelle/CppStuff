#include "ThreadPool.h"
#include <atomic>
#include <algorithm>
#include <limits>

int partialFutex(std::atomic_uint *uaddr, int futex_op, int val)
{
    return syscall(SYS_futex, uaddr, futex_op, val,
                   nullptr, nullptr, 0);
}

void Semaphore::wait(){
    uint localInt = value.load(std::memory_order_relaxed);
    while (localInt == 0
        || !value.compare_exchange_weak(localInt, localInt - 1,
                                                  std::memory_order_acquire,
                                                  std::memory_order_relaxed))
    {
        if (localInt == 0){
            partialFutex(&value, FUTEX_WAIT, 0);
        }
        localInt = value.load(std::memory_order_relaxed);
    }
}

void Semaphore::signal(){
    value.fetch_add(1, std::memory_order_release);
    partialFutex(&value, FUTEX_WAKE, 1);
}

Semaphore::Semaphore(const uint n){
    value = n;
}

ThreadPool::ThreadPool(const int n): semaphore(0){
    threads.reserve(n);
    for (int i = 0; i < n; i++){
        threads.emplace_back(this);
    }
}

void ThreadPool::submitWork(void(*f)()){
    PoolThread::mutex.lock();
    work.push_back(f);
    PoolThread::mutex.unlock();
    semaphore.signal();
}
Mutex print;
void closeThread(){
    print.lock();
    std::cout << "closing thread " << std::this_thread::get_id() << std::endl;
    print.unlock();
}

ThreadPool::~ThreadPool() {
    std::for_each(threads.begin(), threads.end(), [](PoolThread& thread){ thread.runFlag = false;});
    std::for_each(threads.begin(), threads.end(), [this](PoolThread& thread){ this->submitWork(closeThread);});
    std::for_each(threads.begin(), threads.end(), [](PoolThread& thread){ thread.join();});
}