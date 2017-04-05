/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(10000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(1000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // i = rows
    for (int i = 0; i < d; i++)
    {
        // j = columns
        for (int j = 0; j < d; j++)
        {
            // set tile's value
            board[i][j] = ((d*d) - (d*i) - j - 1);
        }
    }
    // If even numbered tiles, swap "1" and "2" tiles which will always be the last second and third-to-last values
    if ((d % 2) == 0)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp; 
    } 
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] > 9)
            {
                printf(" %i ", board[i][j]);
            }
            else if (board[i][j] > 0)
            {
                printf("  %i ", board[i][j]);
            }
            else
            {
                printf("  _ ");
            }
            
        }
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int blanki = 0;
    int blankj = 0;
    int tilei = 0;
    int tilej = 0;
    // Determine blank tile and input tile positions
    for (int i = 0; i < d; i++)
    {
        for (int j= 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                blanki = i;
                blankj = j;
            }
            if (board[i][j] == tile)
            {
                tilei = i;
                tilej = j;
            }
        }
    }
    
    // Determine if move is legal (either above, below, left or right)
    if ((tilei == blanki+1 && tilej == blankj) || (tilei == blanki-1 && tilej == blankj) || (tilei == blanki && tilej == blankj+1) || (tilei == blanki && tilej == blankj-1))
    {
            // Swap tiles
            int temp = board[tilei][tilej];
            board[tilei][tilej] = board[blanki][blankj];
            board[blanki][blankj] = temp;
            return true;
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // Initialise check variable
    int check = 0;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // Add 1 to "check" with each iteration
            check = check + 1;
            // If "check" reaches the total number of tiles, the board is in the correct configuration
            if (check == (d*d))
            {
                return true;
            }
            // If at any stage the numbers are out of order, return false
            if (board[i][j] != check)
            {
                return false;
            }
        }
    }
    // Backup return false JUST IN CASE
    return false;
}
