#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // Get user's name
    string s = GetString();
    
    // Check each letter of name
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        // Automatically capitalise the first letter
        if (i == 0)
        {
            printf("%c", toupper(s[i]));
        }
        // Otherwise capitalise the first letter after a space
        else if ((isspace(s[i]) != 0) && (isspace(s[i+1]) == 0))
        {
            printf("%c", toupper(s[i+1]));
        }
    }
    // New line
    printf("\n");
}