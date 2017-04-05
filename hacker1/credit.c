#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // Get credit card number
    printf("Card number: ");
    long long cc_num = GetLongLong();
    printf("%lld\n", cc_num);
    
    // Determine if card has between 13 and 16 digits
    int nDigits = floor(log10(llabs(cc_num))) + 1;
    printf("%i\n", nDigits);
    if ((nDigits == 13) || (nDigits == 15) || (nDigits == 16))
    {
        printf("VALID\n");
    }
    else
    {
        printf("INVALID\n");
    }
}