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