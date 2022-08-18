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