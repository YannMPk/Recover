#include <cs50.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    typedef uint8_t BYTE;
    BYTE *buffer = malloc(512 * sizeof(BYTE));

    FILE *file = fopen(argv[1], "r");

    //make sure the file is not empty
    if (file == NULL)
    {
        printf("Error Occured opening the raw file.\n");
        return 1;
    }

    // opening the memory card

    unsigned int filecounter;     // counter that lets us know if we have reached the end of the file
    bool foundfirstJPEG = false;   // the variable that tells us we have our first JPEG file
    FILE *current_file;          // the file in which we write
    char current_filename[100];  // the name of the file in which we write
    int currentfilenumber = 0;
    bool startofJPEG = false;      // the variable that tells us we have a JPEG file

    while (true) // repeat until the end of the memory card
    {
        filecounter = fread(buffer, sizeof(BYTE), 512, file);
        if (filecounter == 0)
        {
            break; //we have reached the end of the file (or the file is already empty)
        }

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // verification of the first 4 bytes of the JPEG file
            if (!foundfirstJPEG)
            {
                startofJPEG = true;
                foundfirstJPEG = true;
            }

            else
            {
                //if the condition is not fulfilled, we close the file which is currently being fulfilled
                fclose(current_file);
            }
            sprintf(current_filename, "%0.3i.jpg", currentfilenumber);
            current_file = fopen(current_filename, "w");
            fwrite(buffer, sizeof(BYTE), filecounter, current_file);
            currentfilenumber ++;
        }

        else
        {
            if (startofJPEG)
            {
                fwrite(buffer, sizeof(BYTE), filecounter, current_file);
            }
        }
    }
    free(buffer);
    fclose(file);
    fclose(current_file);
    return 0;
}