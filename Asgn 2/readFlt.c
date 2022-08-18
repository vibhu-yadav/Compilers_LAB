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