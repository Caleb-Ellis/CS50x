#include <cs50.h>
#include <stdio.h>

int GetPyramidInt(void);

int main(void)
{
    int height = GetPyramidInt();
    int floor = 0;
    
    for (int i = 0; i < height; i++)
    {
        int spaces = height - (i + 1);
        for (int j = 0; j < spaces; j++)
        {
            printf(" ");
        }
        for (int k = -2; k < floor; k++)
        {
            printf("#");
        }
        printf("\n");
        floor = floor + 1;
    }
}

int GetPyramidInt(void)
{
    int n;
    do
    {
        printf("Please give me a number of floors between 0 and 23 inclusive: ");
        n = GetInt();
    }
    while (n < 0 || n > 23);
    return n;
}