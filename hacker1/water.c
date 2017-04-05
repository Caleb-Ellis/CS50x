#include <cs50.h>
#include <stdio.h>

int GetPositiveInt(void);

int main(void)
{
    printf("How many minutes long is your shower? ");
    int x = GetPositiveInt();
    
    printf("The equivalent number of water bottles used is %d.\n", x*12);
}

int GetPositiveInt(void)
{
    int n;
    do
    {
        printf("Please give me a positive number: ");
        n = GetInt();
    }
    while (n <= 0);
    return n;
}