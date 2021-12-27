#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(int *arr, int data_num)
{
    int temp;
    for (int i = 0; i < (data_num + 1) / 2 - 1; i++)
    {
        temp = arr[i];
        arr[i] = arr[data_num - 1 - i];
        arr[data_num - 1 - i] = temp;
    }
}

int badcode(const int *arr, int data_num, float *mid)
{
    int smallest = 200, biggest = 0;

    for (int i = 0; i < data_num; i++)
    {
        if (arr[i] > 200 || arr[i] < 1)
            return 1;
        if (arr[i] > biggest)
            biggest = arr[i];
        if (arr[i] < smallest)
            smallest = arr[i];
    }

    *mid = (smallest + biggest) / 2;

    if (biggest / smallest < 1.8)
        return 1;
    else if (biggest / smallest < 2.2)
    {
        for (int i = 0; i < data_num; i++)
        {
            if (arr[i] > smallest * 1.1 && arr[i] < biggest * 0.9025)
                return 1;
        }
        return 0;
    }
    else
        return 1;
}

void translate(int *arr, int data_num, float mid)
{
    for (int i = 0; i < data_num; i++)
    {
        if (arr[i] > mid)
            arr[i] = 1;
        else
            arr[i] = 0;
    }
}

char word(int num)
{
    switch (num)
    {
    case 10000:
        return '0';
        break;
    case 10010:
        return '1';
        break;
    case 11:
        return '2';
        break;
    case 10100:
        return '3';
        break;
    case 1100:
        return '4';
        break;
    case 11000:
        return '5';
        break;
    case 101:
        return '6';
        break;
    case 1001:
        return '7';
        break;
    case 10001:
        return '8';
        break;
    case 1:
        return '9';
        break;
    case 100:
        return '-';
        break;

    default:
        return 'x';
        break;
    }
}
int weight(char ci)
{
    if (ci >= '0' && ci <= '9')
        return (ci - '0');
    else
        return 10;
}

int decode(int *arr, int data_num, char *result, int n)
{
    int temp = 0, c, k;
    if (arr[5] == 1) //分割碼=1
        return 1;
    for (int i = 0; i < n; i++)
    {
        temp = 0;
        for (int j = 0; j < 5; j++)
        {
            temp = (int)arr[i * 6 + 6 + j] + temp * 10;
        }
        if ((int)arr[i * 6 + 6 + 5] == 1) //分割碼=1
            return 1;
        if (word(temp) == 'x')
            return 1;
        else
            result[i] = word(temp);
    }
    // printf("%s\n",result);
    if ((int)arr[data_num - 12] == 1 || (int)arr[data_num - 6] == 1)
        return 1;

    // code C
    temp = 0;
    for (int i = 1; i <= n; i++)
    {
        temp += ((n - i) % 10 + 1) * weight(result[i - 1]);
    }
    c = temp % 11;

    temp = 0;
    for (int j = 0; j < 5; j++)
    {
        temp = (int)arr[data_num - 17 + j] + temp * 10;
    }

    if (word(temp) == 'x')
        return 2;
    else
    {
        if (c != weight(word(temp)))
            return 2;
    }

    // code K
    temp = 0;
    for (int i = 1; i <= n + 1; i++)
    {
        if (i == n + 1)
            temp += ((n - i + 1) % 9 + 1) * c;
        else
            temp += ((n - i + 1) % 9 + 1) * weight(result[i - 1]);
    }
    k = temp % 11;
    temp = 0;
    for (int j = 0; j < 5; j++)
    {
        temp = (int)arr[data_num - 11 + j] + temp * 10;
    }

    if (word(temp) == 'x')
        return 3;
    else
    {
        if (k != weight(word(temp)))
            return 3;
    }

    return 0;
}

int main(void)
{
    int data_num, n, count = 1, state = 0; // state: 0=normal, 1=bad code, 2=bad C, 3=bad K
    float mid;
    char filename[100] = "testfile.txt";
    FILE *input;
    input = fopen(filename, "r");

    fscanf(input, "%d", &data_num);

    while (data_num != 0)
    {
        state = 0;
        int data[150]; // data[data_num]
        n = (data_num + 1) / 6 - 4;
        char result[21]; // result[n]

        for (int i = 0; i < data_num; i++)
        {
            fscanf(input, "%d", &data[i]);
        } // read data

        state = badcode(data, data_num, &mid);

        if (!state)
        {
            translate(data, data_num, mid); // turn in to 1&0

            /*
            for(int i=0;i<data_num;i++){
                printf("%d ",data[i]);
            }
            */

            if (data[0] == 0 && data[1] == 1 && data[2] == 1 && data[3] == 0 && data[4] == 0 && data[data_num - 1] == 0 && data[data_num - 2] == 0 && data[data_num - 3] == 1 && data[data_num - 4] == 1 && data[data_num - 5] == 0)
            {
                reverse(data, data_num);
                state = decode(data, data_num, result, n);
            } //反過來讀

            else if (data[0] == 0 && data[1] == 0 && data[2] == 1 && data[3] == 1 && data[4] == 0 && data[data_num - 1] == 0 && data[data_num - 2] == 1 && data[data_num - 3] == 1 && data[data_num - 4] == 0 && data[data_num - 5] == 0)
            {
                state = decode(data, data_num, result, n);
            } //正著讀

            else
                state = 1; // start/stop error
        }

        // printf("%s\n",result);
        switch (state)
        {
        case 0:
            printf("Case %d: %s\n", count, result);
            break;
        case 1:
            printf("Case %d: bad code\n", count);
            break;
        case 2:
            printf("Case %d: bad C\n", count);
            break;
        case 3:
            printf("Case %d: bad K\n", count);
            break;
        default:
            break;
        }
        count++;
        fscanf(input, "%d", &data_num);
    }
    fclose(input);
    system("pause");
    return 0;
}
