/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

int min(int x, int y)
{
    int ans;
    ans = x > y ? y : x; // ternary
    return ans;
}

int max(int x, int y)
{
    int ans;
    ans = x < y ? y : x; 
    return ans;
}

void print(char *ch)
{
    // print the char array
    printf("%s", ch);
    return;
}

void print_greater(int m, int n)
{
    char ism[] = "m > n";
    char isn[] = "n > m";
    m > n ? print(ism) : print(isn); // ternary
    return;
}

int mul(int a, int b)
{
    int i = min(a, b); // nested function calls
    int j = max(a,b);
    int d = i * i;
    return d;
}

int main()
{
    int a, b, pro;
    a = 1, b = 52;

    print("Function calls and conditional statements");


    pro = mul(a, b);
    
    if( pro > 5 )
        print("Greater than 5.\n");
    else
        print("Less than 5.\n");

    print_greater(b, pro);
    return 0;
}