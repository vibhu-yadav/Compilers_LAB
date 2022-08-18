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