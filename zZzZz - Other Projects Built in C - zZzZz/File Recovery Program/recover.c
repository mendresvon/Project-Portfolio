#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Program should only use
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // Open card file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    }

    BYTE buffer[512];
    int img_count = 0;
    char filename[100];
    FILE *img = NULL;
    // Read 512 bytes into a buffer; Repeat until end of card
    while (fread(buffer, 512, 1, file))
    {
        // Start of new JPEG?
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            sprintf(filename, "%03i.jpg", img_count);
            img = fopen(filename, "w");
            img_count++;
            // Recover the JPEG file
        }
        if (img != NULL)
        {
            fwrite(buffer, 512, 1, img);
        }
    }
    fclose(file);
    fclose(img);

    /*Open Memory Card
    // Repeart until end of card
        // Read 512 bytes into a buffer
        // Start of a new JPEG?
            //yes =>
            //no =>
        // Already found a JPEG?
            //no =>
            //yes =>
    // Close any remaining files
    */
}