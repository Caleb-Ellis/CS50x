#include <cs50.h>
#include <stdio.h>
#include <math.h>

float GetPositiveFloat(void);

int main(void)
{
    float number = GetPositiveFloat();
    int cents = round(number*100);
    int coins = 0;
//    printf("There are %i cents.\n", cents);
    
    for (int i = 0; cents > 0; )
    {
        if (cents >= 25)
        {
            cents = (cents - 25);
            coins = (coins + 1);
        }
        else if (cents >= 10)
        {
            cents = (cents - 10);
            coins = (coins + 1);
        }
        else if (cents >= 5)
        {
            cents = (cents - 5);
            coins = (coins + 1);
        }
        else
        {
            cents = (cents - 1);
            coins = (coins + 1);
        }
        i = i + 1;
    }
    
    printf("%i\n", coins);
}

float GetPositiveFloat(void)
{
    float n;
    do
    {
        printf("Please give me a positive dollar amount: ");
        n = GetFloat();
    }
    while (n < 0);
    return n;
}