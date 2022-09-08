/*
Compilers-Lab Assignment-3
Team Members : Vibhu, Abhibhu Prakash
Roll Numbers: 20CS10072, 20CS10002
*/
#include <stdio.h>
#include <complex.h>

typedef struct{
    char course[15];
    int assignment_no;
}asgn;

typedef enum{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
}weekday;

typedef union {
    int a;
    float b;
    char c;
}uni;

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    asgn current = { .course = "Compilers Lab", .assignment_no = 3};
    weekday today = Wednesday;

    char roll_no[] = "20CS10072";
    int bytes = sizeof(roll_no);
    const int size = bytes / sizeof(char);

    char name[] = "Vibhu";
    char initial = 'A';

    short int age = 20;
    unsigned int year = 2022;
    
    float f = 06.90;
    double d = 123.456;

    char buff[30] = "Next String would be empty.\n\t";

    if(age >= 20){
        printf("You're old enough to do the assignment.\n");
    }
    else{
        printf("Grow up first!\n");
    }

    /*
    This is a multi-line comment that essentially does nothing.
    Even this line is pointless but it had to be there to actually make it a "multi-line" comment.
    */
    switch(year){
        case 2022:
            printf("You're in your 3rd year.\n");
            break;
        
        default:
            printf("You're not in your 3rd year.\n");
            break;
    }

    void *ptr = NULL;

    do{
        printf("This is a do-while loop.\n");
    }
    while(ptr != NULL);

    _Bool flag = 1;
    _Complex float c = 1.0 + 2.0 * I;

    auto auto_var = "This is an auto variable.";
    static const double e = 2.7818284590;

    return 0;
}