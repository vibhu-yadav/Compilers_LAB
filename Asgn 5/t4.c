/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

int exp(int n, int p)
{
    if (p == 0)
        return n;
    else if (p % 2)
    {
        int temp = exp(n, p / 2);
        return temp * temp * n;
    }

    else
    {
        int temp = exp(n, p / 2);
        return temp * temp;   
    }
}

int main()
{

    printf("Testing nested if else and recursive functions.\n");

    int n;
    int p = 12;
    n = 4;

    if (n > 10)
    {
        n = 10;
    }
    else
    {
        if (n < 0) // nested if-else
        {
            n = 0;
        }
        else
        {
            n = 5;
        }
    }
    int exponent = exp(n, p);
    return 0;
}