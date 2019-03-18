#include <thread>
#include "ThreadPool.h"
#include <iostream>

using namespace std;
    
extern Mutex print;

void work1(){
    print.lock();
    cout << "work1" << endl;
    print.unlock();
}

void work2(){
    print.lock();
    cout << "work2" << endl;
    print.unlock();
}

int main(int argc, char** argv) {
    ThreadPool pool(2);
    pool.submitWork(work1);
    pool.submitWork(work2);
    pool.submitWork(work1);
    pool.submitWork(work2);
    usleep(2000000);
    pool.submitWork(work1);
    pool.submitWork(work2);
    pool.submitWork(work1);
    pool.submitWork(work2);
    usleep(2000000);
    pool.submitWork(work1);
    pool.submitWork(work2);
    pool.submitWork(work1);
    pool.submitWork(work2);
    usleep(2000000);
    return 0;
}

