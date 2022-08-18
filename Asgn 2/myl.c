#include "myl.h"
#define BUFF 20
#define PRECISION 6

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

    return OK;

}

// In C, size of int datatype is 4 bytes.
// Thus, a int variable has to lie in range of -2^31 to 2^31-1.
// In decimal notation, this is range is [-2,147,483,648 ,2,147,483,647].
// Thus a int variable can be string of atmost length 11 (10 digits + 1 sign).
// Therefore a 20 character buffer is sufficient to store a int variable.
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

    buff[i] = '\n';
    bytes = i+1;

    bytes = i;

    __asm__ __volatile__ (
        "movl $1, %%eax\n\t"
        "movq $1, %%rdi\n\t"
        "syscall \n\t"
        :
        :"S"(buff),"d"(bytes)
    ); 

    return OK;

}

int readInt(int *n){
    
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
        return ERR;

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
            return ERR;
        }
    }

    if(neg) ans = -ans;

    *n = ans;

    return OK;

}

int readFlt(float *f){

    char buff[BUFF]; // buffer to store input, max size is BUFF and '\n' as terminator.
    int i = 0; // for iterating over buff
    int size; // for storing size of buff
    float ans = 0; // for storing answer to return
    int neg = 0; // 0 for positive, 1 for negative.

    __asm__ __volatile__ (
        "movl $0, %%eax\n\t"
        "movq $0, %%rdi\n\t"
        "syscall \n\t"
        : "=a" (size)
        :"S"(buff),"d"(BUFF)
    );

    if(size < 0)
        return ERR;

    if(buff[i] == '-'){
        neg = 1;
        i++;
    }

    int integer = 0;
    // Iterating over buff till '.' is found.
    while(i < size && buff[i] != '.'){
        if(buff[i] >= '0' && buff[i] <= '9'){
            integer = integer * 10 + (buff[i] - '0');
            i++;
        }
        else{
            return ERR;
        }
    }

    if(neg) integer = -integer;

    i++; // Incrementing i to point to next character after '.'

    int fractional = 0;
    int digits = 0;

    while(i < size && buff[i] != '\n'){
        if(buff[i] >= '0' && buff[i] <= '9'){
            fractional = fractional * 10 + (buff[i] - '0');
            i++;
            digits++;
        }
        else{
            return ERR;
        }
    }

    float decimal = (float) (fractional);
    while(digits--){
        decimal /= 10;
    }

    *f = ( (float)integer) + decimal;

    return OK;

}

// This version of printFlt depends on the implementation of 
// printInt and printStr.

/*
int printFlt(float f){

    int whole = (int) f;
    f -= whole;

    if(f < 0) f = -f;


    printInt(whole);

    char* buff = ".";
    int bytes = 1;
    __asm__ __volatile__ (
        "movl $1, %%eax\n\t"
        "movq $1, %%rdi\n\t"
        "syscall \n\t"
        :
        :"S"(buff),"d"(bytes)
    );

    for(int i=0;i<PRECISION;i++) f *= 10;

    int fractional = (int) f;

    printInt(fractional);

    return OK;

}
*/

int printFlt(float f){
    
        char buff[BUFF];
        int i = 0,bytes;
        float eps = 1e-9;

        if((f+eps) > 0 && (f-eps) < 0){
            buff[i++] = '0';
            buff[i++] = '.';
            buff[i++] = '0';
        }
        else{
            int j,k;

            if(f < 0){
                buff[i++] = '-';
                f = -f;
            }

            int whole = (int) f;
            f -= whole;

            while(whole){
                int dig = whole % 10;
                buff[i++] = (char) (dig + '0');
                whole /= 10;
            }

            if(buff[0] == '-') j = 1;
            else j = 0;

            k = i-1;

            while(j < k){
                char temp = buff[j];
                buff[j++] = buff[k];
                buff[k--] = temp;
            }

            buff[i++] = '.';

            for(int j=0;j<PRECISION;j++){
                f *= 10;
                int dig = (int) f;
                buff[i++] = (char) (dig + '0');
                f -= dig;
            }

        }

        buff[i] = '\n';
        bytes = i+1;

        __asm__ __volatile__ (
        "movl $1, %%eax\n\t"
        "movq $1, %%rdi\n\t"
        "syscall \n\t"
        :
        :"S"(buff),"d"(bytes)
        ); 

    return OK;    
}
