#include <thread>
#include "ThreadPool.h"
#include <iostream>

using namespace std;

    int futexVal = 9001;
    
void work1(){
    cout << "work1" << endl;
}

void work2(){
    cout << "work2" << endl;
}

int main(int argc, char** argv) {    
    ThreadPool pool(2);
    pool.doWork(work1);
    pool.doWork(work2);
    usleep(1000000);
    pool.shutdown();
    return 0;
}

