# define BUFF 20
# define ERR 0
# define OK 1

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

int main(){

    int loop;
    float f;

    do {
        printStr("Enter a float: ");
        int ans = readFlt(&f);
        if(ans == ERR) 
            printStr("Invalid input. ");
        else{
            printStr("Successfuly read float: ");
            printFlt(f);
        }
        printStr("\nEnter -1 to terminate, anything else to continue: ");
        readInt(&loop);
    } while(loop != -1);
}