#include "myl.h"

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