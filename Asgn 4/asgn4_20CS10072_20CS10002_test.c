/*
This is a multi-line comment.
Assignment-4 Test File
Abhibhu Prakash - 20CS10002
Vibhu           - 20CS10072
*/

extern int some_random_variable;

inline void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{

    // Checking keywords, identifiers and constants (also a single line comment.)

    char string = "Hello Tester, this is a test string";

    auto a = 20;
    char cs[] = "CS";
    float f = 100.72;
    double d = 17.39e3;

    static const long double euler = 2.718281828459045;
    volatile short int v = 10;

    enum days
    {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    } day = Wednesday;

    char newline = '\n';

    int x = 10, y = 20;
    swap(&x, &y);

    // Checking Arithmetic Operations

    int t = x + y;
    t = x - y;
    t = x * y;
    t = x / y;
    t = x % y;
    t = x++;
    t = x--;
    t = ++x;
    t = --x;
    t = -x;
    t = +x;
    t = x << 3;
    t = x >> 3;
    t = x & y;
    t = x | y;
    t = x ^ y;
    t = ~x;
    t = !x;
    t = (x == y) ? x : y;

    // Checking Assignment Operations

    t += x;
    t -= x;
    t *= x;
    t /= x;
    t %= x;
    t <<= 5;
    t >>= 5;
    t &= x;
    t ^= x;
    t |= x;

    for (int i = 0; i < 14; i++) // for loop
    {
        if (i == day || day > 7) 
            break;

        else
        {
            int j = i * 7;

            do{
                j++;
            }
            while (j % 3 != 0);
        }
    }

    return 0;
}