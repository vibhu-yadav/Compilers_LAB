#include "myl.h"

int main(){

    // Null terminated string
    char *test_str = "Hello World";
    int test_int = 10;
    float test_flt = -1.2345;

    // printStr(test_str);
    // printStr("\n");

    // printInt(10);
    // printStr("\n");

    // printFlt(test_flt);
    // printStr("\n");

    
    // printStr("Enter a Integer: ");
    // printf("%d",readInt(&test_int));

    // printInt(test_int);

    printStr("Enter a Float: ");
    readFlt(&test_flt);

    printFlt(test_flt);

    // printStr("\n");

    return 0;
}