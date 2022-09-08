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

    if(neg) ans = -ans;

    *n = ans;

    return OK;

}

/*
int readFlt(float *f){

    char buff[BUFF]; // buffer to store input, max size is BUFF and '\n' as terminator.
    int i = 0; // for iterating over buff
    int size; // for storing size of buff
    // float ans = 0; // for storing answer to return
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

    return size;

}


*/

int readFlt(float *f){
    char buff[BUFF];
    int len;

    __asm__ __volatile__ (
        "movl $0, %%eax \n\t" 
        "movq $0, %%rdi \n\t"
        "syscall \n\t"
        : "=a"(len)
        :"S"(buff), "d"(BUFF));

    if(len < 0)
        return ERR;
    
    int sign = 1, index = 0, multiplier = 0, msign = 1;
    float ret = 0;
    if(index < len) {
        if(buff[index] == '-') {
            sign = -1;
            index++;
        } else if(buff[index] == '+') {
            index++;
        }
    }
    
    while(index < len && buff[index] != '\n' && buff[index] != '.' ) {
        if(buff[index] < '0' || buff[index] > '9')
            return ERR;
        int curr = (int)(buff[index] - '0');
        ret *= 10;
        ret += curr;
        index++;
    }

    if(index < len && buff[index] == '.') {
        index++;
        float shift = 10.F;
        while(index < len && buff[index] != '\n' ) {
            if(buff[index] < '0' || buff[index] > '9')
                return ERR;
            float curr = (float)(buff[index] - '0');
            curr /= shift;
            ret += curr;
            shift *= 10;
            index++;
        }
    }


    for(int i = 0; i < multiplier; i++) {
        if(msign == 1) {
            ret *= 10;
        } else {
            ret /= 10;
        }
    }
    *f = (sign * ret);
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


#define INT_MAX __INT32_MAX__
#define INT_MIN (-INT_MAX - 1)

int main(){

    printStr("\n#### Testing printStr ####\n");
    char *s[3] = {"Is this a sample string?", "Probably yes. Btw the next one is an empty string.", ""};

    for(int i=0; i<3; i++){
        printStr("\""); 
        int ret = printStr(s[i]); 
        printStr("\" => printed ");
        printInt(ret);
        printStr(" characters\n");
    }

    printStr("\n\n#### Testing printInt ####\n");
    int ints[11] = {0, 1, -1, 12, -12, 1234, -1234, 12345, -12345, INT_MAX, INT_MIN+1};

    for(int i=0; i<11; i++){
        int ret = printInt(ints[i]);
        printStr("\" => printed ");
        printInt(ret);
        printStr(" characters\n");
    }


    printStr("#### Testing printFlt ####\n");
    float floats[10] = {0.F, -12.34F, 12.345F, -2.F, 2.F, 0.1234F, -0.1234F, 123.456F, 0.001F, -0.0106F};

    for(int i=0; i<10; i++){
        int ret = printFlt(floats[i]);
        printStr("\" => printed ");
        printInt(ret);
        printStr(" characters\n");
    }

    printStr("\n\n#### Testing readInt ####\n");
    int n;
    int loop = 0;

    do {
        printStr("Enter an integer: ");
        int ret = readInt(&n);
        if(ret == ERR) 
            printStr("Invalid input. ");
        else{
            printStr("Successfuly read integer: ");
            printInt(n);
        }
        printStr("\nEnter -1 to terminate, anything else to continue: ");
        readInt(&loop);
    } while(loop != -1);

    printStr("\n\n#### Testing readFlt ####\n");
    float f;
    loop = 0;

    do {
        printStr("Enter a float: ");
        int ret = readFlt(&f);
        if(ret == ERR) 
            printStr("Invalid input. ");
        else{
            printStr("Successfuly read float: ");
            printFlt(f);
        }
        printStr("\nEnter -1 to terminate, anything else to continue: ");
        readInt(&loop);
    } while(loop != -1);

    return 0;
}