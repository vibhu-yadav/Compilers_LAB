/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

#include "myl.h"
#define BUFF 100
#define INT_MAX __INT32_MAX__
#define INT_MIN -INT_MAX

int printStr(char* str){

    if(!str)
        return ERR;

    int bytes=0;

    while(str[bytes] != '\0'){
        bytes++;
    }

    __asm__ __volatile__ (
        "movl $1, %%eax\n\t"
        "movq $1, %%rdi\n\t"
        "syscall \n\t"
        :
        :"S"(str),"d"(bytes)
    );

    return bytes;

}

int readInt(int *ep){
    
    char buff[BUFF]; // buffer to store input, max size is BUFF and '\n' as terminator.
    int i = 0; // for iterating over buff
    int size; // for storing size of buff
    int ans = 0; // for storing answer to return
    int neg = 0; // 0 for positive, 1 for negative.

    __asm__ __volatile__ (
        "movl $0, %%eax\n\t"
        "movq $0, %%rdi\n\t"
        "syscall \n\t"
        : "=a" (size)
        :"S"(buff),"d"(BUFF)
    );

    if(size < 0)
        *ep = ERR;

    if(buff[i] == '-'){
        neg = 1;
        i++;
    }

    while( i < size  && buff[i] != '\n'){
        if(buff[i] >= '0' && buff[i] <= '9'){
            ans = ans * 10 + (buff[i] - '0');
            i++;
        }
        else{
            *ep = ERR;
        }
    }

    if(ans > INT_MAX || ans < INT_MIN)
        *ep = ERR;

    if(neg) ans = -ans;

    return ans;
    
}

int printInt(int n){

    char buff[BUFF], zero = '0';
    int i = 0, j,k, bytes;

    if(n == 0) buff[i++] = zero;
    else{
        if(n<0){
            buff[i++] = '-';
            n = -n;
        }    
        while(n){
            int dig = n % 10;
            buff[i++] = (char) (dig + zero);
            n /= 10;
        }

        if(buff[0] == '-') j = 1;
        else j = 0;

        k = i-1;

        while(j < k){
            char temp = buff[j];
            buff[j++] = buff[k];
            buff[k--] = temp;
        }
    }

    bytes = i;

    __asm__ __volatile__ (
        "movl $1, %%eax\n\t"
        "movq $1, %%rdi\n\t"
        "syscall \n\t"
        :
        :"S"(buff),"d"(bytes)
    ); 

    return bytes;

}