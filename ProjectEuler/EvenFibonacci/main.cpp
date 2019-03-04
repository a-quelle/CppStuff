#include "stdio.h"
using namespace std;

int acc = 0;

void recursion(int a, int b){
    int c = a+b;
    if(a+b <= 4000000 ){
        if((c&1) == 0)
            acc += c;
        recursion(b,c);
    }        
}
int main(int argc, char** argv) {
    
    recursion(1,1);
    printf("The sum of all even Fibonacci numbers <= 4000000 = %d \n", acc);
    return 0;
}

