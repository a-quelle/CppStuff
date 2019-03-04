#include "stdio.h"
#include <cstdlib>

int sieve[1000000];

void fillSieve(){    
    for(int i = 0; i < 1000000; i++){
       sieve[i] = 1;
    }
    sieve[0] = 0;
}

void emptySieve(){
    for(int i = 2; i <= 500000; i++){
        int end = 1000000 / i;
        for(int j = 2; j <= end; j++)
            sieve[i*j-1] = 0;
    }
}

int divides600851475143(int n){
    return 600851475143%n == 0;
}

int main(int argc, char** argv) {
    fillSieve();
    emptySieve();
    int index = 999999;
    while(1){
        if(sieve[index] && divides600851475143(index+1))
            break;
        index--;
    }
    printf("The largest prime factor of 600851475143 is: %d \n", index +1);
    return 0;
}

