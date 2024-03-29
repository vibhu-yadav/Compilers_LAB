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