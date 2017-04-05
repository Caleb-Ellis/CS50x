#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, string argv[])
{
     if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }
    int d = atoi(argv[1]);
    int grid[d][d];
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // set tile's value
            grid[i][j] = ((d*d) - (d*i) - j - 1);
        }
    }
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (grid[i][j] > 9)
            {
                printf("%i ", grid[i][j]);
            }
            else if (grid[i][j] > 0)
            {
                printf(" %i ", grid[i][j]);
            }
            else
            {
                printf(" _");
            }
            
        }
        printf("\n");
    }
}