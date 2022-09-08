#include "myl.h"
#define BUFF 20
#define PRECISION 6
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

    if(ans > INT_MAX || ans < INT_MIN)
        return ERR;

    if(neg) ans = -ans;

    *n = ans;

    return OK;
}

int readFlt(float *f){
    char buff[BUFF];
    int size;

    __asm__ __volatile__ (
        "movl $0, %%eax \n\t" 
        "movq $0, %%rdi \n\t"
        "syscall \n\t"
        : "=a"(size)
        :"S"(buff), "d"(BUFF));

    if(size < 0)
        return ERR;
    
    int neg = 0; // To note if the number is negative.
    int i = 0;
    float ans = 0;
    
    if(i < size && buff[i] == '-') {
        neg = 1;
        i++;
    }
    
    while(i < size && buff[i] != '\n' && buff[i] != '.' ) {
        
        if(buff[i] < '0' || buff[i] > '9')
            return ERR;

        int curr = (int)(buff[i] - '0');
        ans = ans * 10 + curr;
        i++;
    }

    if(i < size && buff[i] == '.') {
        i++;
        float den = 10.F; // Denominator.
        while(i < size && buff[i] != '\n' ) {

            if(buff[i] < '0' || buff[i] > '9')
                return ERR;

            float curr = (float)(buff[i] - '0');
            curr /= den;
            ans += curr;
            den *= 10;
            i++;
        }
    }

    
    *f = ans;

    if(neg){
        *f *= -1;
    }
    
    return OK;
}

int printFlt(float f){
    
    char buff[BUFF];
    int i = 0,bytes;
    float eps = 1e-6;

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

    bytes = i;

    __asm__ __volatile__ (
    "movl $1, %%eax\n\t"
    "movq $1, %%rdi\n\t"
    "syscall \n\t"
    :
    :"S"(buff),"d"(bytes)
    ); 

    return i;    
}

