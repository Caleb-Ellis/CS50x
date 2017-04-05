#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int BLOCKSIZE = 512;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover filename\n");
        return 1;
    }
    
    // Open input file
    FILE *rawfile = fopen(argv[1], "r");
    if (rawfile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    

    uint8_t buffer[BLOCKSIZE];    //set buffer as blocksize
    int counter = 0;    //current JPEG counter
    int open = 0;   //open outfile indicator
    FILE *jaypeg;  //declare outfile
    
    // Iterate over file contents, one block at a time
    while (fread(buffer, BLOCKSIZE, 1, rawfile))
    {
        // Check if first four bytes of block are start of JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0)
            {
                char filename[8];
                sprintf(filename, "%03i.jpg", counter);
                
                // If a JPEG is currently is NOT being read
                if (open == 0)
                {
                    jaypeg = fopen(filename, "w");
                    fwrite(buffer, BLOCKSIZE, 1, jaypeg);
                    open = 1;
                }
                
                // If a JPEG currently IS being read
                if (open == 1)
                {
                    fclose(jaypeg);
                    jaypeg = fopen(filename, "w");
                    fwrite(buffer, BLOCKSIZE, 1, jaypeg);
                    counter++;
                }
                
            }
        else
        {
            if (open == 1)
                fwrite(buffer, BLOCKSIZE, 1, jaypeg);
        }
            
    }
    fclose(rawfile);
    fclose(jaypeg);
}