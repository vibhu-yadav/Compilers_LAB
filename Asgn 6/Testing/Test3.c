int printStr(char *ch);
int printInt(int n);
int readInt(int *eP);

int FirstNSum(int N); // Testing function declaration

int main()
{
    int sum;
    sum = FirstNSum(5);
    printStr("Sum of first 5 integers is:");
    printInt(sum);
    printStr("\n");

    // Scope testing
    printStr("Testing the scope of a variable inside a loop:\n");
    int a, b, c;
    int temp;
    printStr("Input 3 numbers:\n");
    a = readInt(&temp);
    b = readInt(&temp);
    c = readInt(&temp);
    {
        int k = a;
        printStr("\nScope 1: ");
        printInt(k);
        {
            int k = b;
            printStr("\nScope 2: ");
            printInt(k);
            {
                int k = c;
                printStr("\nScope 3: ");
                printInt(k);
                printStr("\n");
            }
        }
    }
    return 0;
}

int FirstNSum(int N)
{
    if (N <= 1)
        return N;

    return N + FirstNSum(N - 1);
}