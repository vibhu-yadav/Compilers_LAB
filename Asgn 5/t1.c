/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

// Global declarations
float pi = 3.1416;
char c;
int sum, diff, div, pro, rem, or, and, t1, t2;
int w[10];        // 1D array declaration
float f[20][50];  // 2D array declaration
int a = 4, *p, b; // pointer declaration

void func(int i, float d)
{
    d = (float)(i ^ 7);
    return;
}

int main()
{

    printf("Declarations and arithmetic operations.\n");

    // character definition
    char ch = 'c', str = 'a';
    // Variable Declaration
    int x = 169;
    int y = 13;

    // Arithmetic Operations
    t1 = x << 2; // shift left
    t2 = y >> 1; // shift right

    sum = x + y;  // addition
    diff = x - y; // subtraction
    div = x / y;  // division
    pro = x * y;  // multiplication
    rem = x % y;  // modulus
    or = x | y;   // bitwise or
    and = x & y;  // bitwise and

    return 0;
}