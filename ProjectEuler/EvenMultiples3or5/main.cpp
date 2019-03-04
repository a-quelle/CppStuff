#include "stdio.h"

using namespace std;

int divBy3(int n){
    while(n > 3){
        int d = 0;
        while(n>0){
            d += n & 3;
            n >>= 2;
        }
        n = d;
    }
    return n == 3;
}

int main(int argc, char** argv) {
    int acc = 0;
    for(int i = 1; i <= 333; i++){
        acc += 3*i;
    }
    for(int i = 1; i < 200; i++){
        if(!divBy3(i))
            acc += 5*i;        
    }
    printf("The sum of all numbers divisible by 3 or 5 is: %d \n", acc);    
    return 0;
}

