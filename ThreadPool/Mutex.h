#ifndef MUTEX_H
#define MUTEX_H
#include <atomic>

typedef void (*voidFunc)();

class Mutex {
public:
    Mutex();
    Mutex(const Mutex& orig) = delete;
    Mutex(Mutex&& orig) = default;
    Mutex& operator=(const Mutex& orig) = delete;
    Mutex& operator=(Mutex&& orig) = default;
    ~Mutex();
    void lock();
    void unlock();
private:
    std::atomic_flag _lock;
};

#endif /* MUTEX_H */

