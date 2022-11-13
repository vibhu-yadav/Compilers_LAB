int printStr(char *ch);
int printInt(int n);
int readInt(int *eP);

int pow(int a, int exp); // Testing function declaration

int main()
{
    int a, exp;
    int temp;
    printStr("This algorithm is for finding the desired exponent of a number bounded within the range of a 4 bytes integer\n"); // Testing printStr
    printStr("Input the base for calculating the exponent (between -10 to 10):");
    a = readInt(&temp); // Testing readInt
    printStr("\n");
    if (a > 10 || a < -10)
        do
        {
            printStr("Input is out of range! Try again. \n");
            printStr("Input the base for calculating the exponent (between -10 to 10):");
            a = readInt(&temp);
            printStr("\n");
        } while (a > 10 || a < -10);

    printStr("Input the exponent value for calculating exponent of the number (between 0 to 9):");
    exp = readInt(&temp);
    printStr("\n");
    if (exp > 9 || exp < 0)
        do // Testing do-while loop
        {
            printStr("Input is out of range! Try again. \n");
            printStr("Input the exponent value for calculating exponent of the number (between 0 to 9):");
            exp = readInt(&temp);
            printStr("\n");
        } while (exp > 9 || exp < 0);

    int ans = pow(a, exp);
    printStr("The value for the input exponent is:");
    printInt(ans);
    return 0;
}

int pow(int a, int exp) // Testing recursive functions
{
    if (exp == 1) // Testing conditional statements
        return a;
    else if (exp == 0)
        return 1;
    else if (exp % 2 == 1)
    {
        int temp;
        temp = pow(a, exp / 2) * pow(a, exp / 2) * a; // Testing recursion
        return temp;
    }
    else
    {
        int temp;
        temp = pow(a, exp / 2) * pow(a, exp / 2); // Testing recursion
        return temp;
    }
}