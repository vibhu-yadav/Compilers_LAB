/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

int findMax(int arr[10])
{
    int temp = arr[0];
    for (int i = 0; i < 10; i++)
    {
        if (temp < arr[i])
            temp = arr[i];
    }
    return temp;
}

int sum(int arr[][5])
{
    int temp[5];
    int sum = 0;
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            sum += arr[j][i];
        }
    }
    return sum;
}

int main()
{

    printf("Testing Arrays and Loops.\n");

    int p[5]; // 1D integer array
    int j = 0;
    do // do-while loop
    {
            p[j++] = p[j] * j;
    } while (j < 5);

    int arr[5][5];
    int i = 0;
    while (i < 5) // while loop
    {
        for (int j = 0; j < 5; j++)
            arr[j][i] = j + i;
        i++;
    }

    int temp;
    temp = sum(arr);

    return 0;
}