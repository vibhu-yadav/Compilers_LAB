#include <stdio.h>

extern int yyparse();

int main() 
{
    printf("==================== Parsing  test_file ====================\n");
    
    yyparse();

    printf("================== Parsing done Successfuly ==================\n");


    return 0;
}