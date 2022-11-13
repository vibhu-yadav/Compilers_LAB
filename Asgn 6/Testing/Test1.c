int printStr(char *ch);
int printInt(int n);
int readInt(int *eP);

int p, q, r, s, k;
float f;
char ch;
int w[10];
int *ptr;

int main()
{
    int x;
    int y;
    int temp;
    printStr("Enter x: ");
    x = readInt(&temp);
    printStr("Enter y: ");
    y = readInt(&temp);
    char ch = 'c';

    p = x + y; // Addition
    printStr("Testing addition\n");
    printStr("p = x + y = ");
    printInt(p);
    printStr("\n");
    printStr("Addition. Check\n");

    q = x - y; // Subtraction
    printStr("Testing Subtraction\n");
    printStr("q = x - y = ");
    printInt(q);
    printStr("\n");
    printStr("Subtraction. Check\n");

    r = x * y; // Multiplication
    printStr("Testing Multiplication\n");
    printStr("r = x * y = ");
    printInt(r);
    printStr("\n");
    printStr("Multiplication. Check\n");

    s = x / y; // Division
    printStr("Testing Division\n");
    printStr("s = x / y = ");
    printInt(s);
    printStr("\n");
    printStr("Division. Check\n");

    k = x % y; // Modulo
    printStr("Testing Modulo\n");
    printStr("k = x % y = ");
    printInt(k);
    printStr("\n");
    printStr("Modulo. Check\n");

    printStr("Arithmetic operations. Complete\n");

    return 0;
}