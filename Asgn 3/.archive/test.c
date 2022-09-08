/*
Compilers-Lab Assignment-3
Team Members : Vibhu, Abhibhu Prakash
Roll Numbers: 20CS10072, 20CS10002
*/
#include <stdio.h>
#define ASSGN 3
struct student{
    int roll;
    char name[30];
};

enum week {monday, tuesday, wednesday, thursday, friday, saturday, sunday};


void main()
{
    int n = 20123;
    char ch = 'CS';
    float f = 1.00;
    double d = 7.2;
    float t = 10.002;
    char buff[30] = "Next String would be empty.\n\t";
    char arr[4] = "";
    /* this comment is a multi line comment. 
    We will briefly check one single line just after this! */

    enum week day;
    day = wednesday;

    printf("Today is %d.\n",day);

    do
    {
        n -= 1;
    }while(n > 1);

    // This single line comment will be followed by a more useful single line comment that explains what the code below does.
    // If t<31, increment n, else decrement it.
    if(t <= 30)
        n++;
    else
        n--;
    printf("print: %d\n", n+1);
}