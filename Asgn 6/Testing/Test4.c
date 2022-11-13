int printStr (char *ch);
int printInt (int n);
int readInt (int *eP);

int main()
{
    printStr("What is 1000 - 7? \n\n"); // Test iteration with a do while loop
    int i = 1000;
    int j = 0;
    do  // Decrementor
    {   // Do-while loop
        printInt(++j);
        printStr(") ");
        printInt(i -7);
        i = i - 7;
        printStr("\n");
    } while (i > 0);
    return 0;
}