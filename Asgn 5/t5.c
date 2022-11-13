
/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

void isPos(float *i)
{
    int temp = *i;
    if (temp < 0)
        *i = -temp;
    return;
}

void charcast(float *i) // pointers
{
    isPos(i);
    char temp = (char)*i;
    temp %= 255;
    *i = temp;
    return;
}

void typecast(float *f)
{
    int temp = (int)*f;
    *f = temp;
}

int main()
{

    printf("Testing Typecasting and pointer logic. \n");

    float arr[6];
    arr[0] = 32.657;
    arr[1] = 567.102;
    arr[2] = 41.0009;
    arr[3] = 9;
    arr[4] = 92.7489;
    arr[5] = 7982.2645;

    for (int i = 0; i < 6; i++)
        typecast(&arr[i]);

    for (int i = 0; i < 6; i++)
        charcast(&arr[i]);

    return 0;
}