// Testing single line comment
/* Testing multiline comment
Hope this line does not get printed
If you see this line in output, just know that multi-line comment did not work as intended */

int printStr(char *ch);
int printInt(int n);
int readInt(int *eP);

int swapcount = 0; // Testing global variable

int bubblesort(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        int j;
        for (j = 1; j < n; j++) // Testing nested for loop
        {
            if (arr[j] < arr[j - 1])
            {
                int temp;
                temp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = temp;
                swapcount++;
            }
        }
    }
    return 0; // Testing return statement
}

int main()
{
    int i, n, flag; // Testing variable declaration
    int arr[10];    // Testing 1D array declaration

    printStr("Enter 10 array elements to perform bubble sort, separated by newlines:\n");
    for (i = 0; i < 10; i++)
    {
        arr[i] = readInt(&n); // Testing readInt
    }

    printStr("You entered the following array:\n");
    for (i = 0; i < 10; i++) // Testing for loop
    {
        printInt(arr[i]); // Testing printInt
        printStr(" ");    // Testing printStr
    }
    printStr("\n");

    flag = 1;
    int flag = bubblesort(arr, 10);

    if (flag == 0)
        printStr("BUBBLESORT SUCCESS!!  ;)\n");

    printStr("Here is the sorted array:\n");
    for (i = 0; i < 10; i++) // Testing for loop
    {
        printInt(arr[i]); // Testing printInt
        printStr(" ");    // Testing printStr
    }
    printStr("\n");

    printStr("The number of steps involved is:");
    printInt(swapcount);
    return 0;
}