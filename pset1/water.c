#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("How many minutes long is your shower? ");
    int x = GetInt();
    
    printf("The equivalent number of water bottles used is %d.\n", x*12);
}