#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Error if not 2 arguments
    if (argc != 2)
    {
        printf("Require two arguments.\n");
        return 1;
    }
    
    string k = argv[1];
    
    // Error if keyword is non-alphabetical
    for (int i = 0, n = strlen(k); i < n; i++)
    {
        if (isalpha(k[i]) == 0)
        {
            printf("Keyword must be alphabetical ONLY.\n");
            return 1;
        }
        // Convert key to all caps since they function identically anyway
        else
        k[i] = toupper (k[i]);
    }
    
  //printf("Key: %s\nString: ", k);
    string s = GetString();
    int j = 0;
    
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        // j is the position of the key, and loops with the % command
        j = j % strlen(k);
        if (isalpha(s[i]))
        {
            int key = k[j] - 65;
            // Check if upper or lower case - alter result accordingly using ASCII math
            if (isupper(s[i]))
            {
                char c = ((s[i] + key - 65) % 26);
                printf("%c", c + 65);
            }
            else if (islower(s[i]))                
            {
                char c = ((s[i] + key - 97) % 26);
                printf("%c", c + 97);
            }
            // Move position of j one forward
            j = j + 1;
        }
        // If non-alphabetic, print unchanged
        else
        printf("%c", s[i]);
    }
    printf("\n");
    return 0;
}