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