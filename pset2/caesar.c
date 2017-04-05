#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // If there are non-2 arguments, give error
    if (argc != 2)
    {
        printf("Require ONLY ONE Caesar key.\n");
        return 1;
    }
    else
    {
        // Convert command prompt string into int
        int k = atoi(argv[1]);
        string s = GetString();
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            // Check if the character is a letter
            if (isalpha(s[i]))
            {
                // Check if upper or lower case
                if (isupper(s[i]))
                {
                    char c = (((s[i] + k) - 65) % 26);
                    printf("%c", c + 65);
                }
                else
                {
                    char c = (((s[i] + k) - 97) % 26);
                    printf("%c", c + 97);
                }
            }
            // If non-alphabetic, print unchanged
            else
            printf("%c", s[i]);
        }
        printf("\n");
        return 0;
    }
}    