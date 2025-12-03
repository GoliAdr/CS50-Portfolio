#include <stdio.h>      // fopen, fread, fwrite, fclose, printf
#include <stdlib.h>     // exit codes
#include <stdint.h>     // uint8_t type (unsigned 8-bit byte)
#include <stdbool.h>    // bool type
#include <string.h>     // sprintf

// Size of each block on the memory card (per spec)
#define BLOCK_SIZE 512  // read the file in chunks of 512 bytes

// Helper: check if a 512-byte block starts a new JPEG
static bool is_jpeg_header(uint8_t buffer[BLOCK_SIZE]) // true if header matches FF D8 FF E0..EF
{
    return buffer[0] == 0xFF                            // first byte  0xFF
        && buffer[1] == 0xD8                            // second     0xD8
        && buffer[2] == 0xFF                            // third      0xFF
        && (buffer[3] & 0xF0) == 0xE0;                  // fourth top nibble 0xE (E0..EF)
}

int main(int argc, char *argv[])
{
    // Usage check: exactly one argument (the raw file)
    if (argc != 2)                                      // need 1 arg: the input filename
    {
        printf("Usage: ./recover card.raw\n");          // guide the user
        return 1;                                       // non-zero means error
    }

    // Open the input file for reading
    FILE *in = fopen(argv[1], "rb");                    // "rb" = read binary
    if (in == NULL)                                     // failed to open
    {
        printf("Could not open input file.\n");         // error message
        return 1;                                       // stop
    }

    uint8_t buffer[BLOCK_SIZE];                         // 512-byte buffer
    FILE *out = NULL;                                   // current JPEG file we’re writing (if any)
    int img_count = 0;                                  // how many JPEGs we’ve found
    char filename[8];                                   // "000.jpg" (7 chars + '\0')

    // Read the raw file 512 bytes at a time
    while (fread(buffer, 1, BLOCK_SIZE, in) == BLOCK_SIZE) // while we can read a full block
    {
        if (is_jpeg_header(buffer))                     // new JPEG starts here?
        {
            if (out != NULL)                            // already writing a JPEG?
            {
                fclose(out);                            // close the previous one
                out = NULL;                             // clear pointer
            }

            sprintf(filename, "%03i.jpg", img_count);   // make name like 000.jpg, 001.jpg, ...
            out = fopen(filename, "wb");                // open new JPEG for writing
            if (out == NULL)                            // check open succeeded
            {
                printf("Could not create output file.\n");
                fclose(in);
                return 1;
            }

            img_count++;                                // we’ve started a new image
        }

        if (out != NULL)                                // if we are currently in a JPEG (new or ongoing)
        {
            fwrite(buffer, 1, BLOCK_SIZE, out);         // write this 512-byte block to the JPEG
        }
        // if out is NULL and no header found yet, we just skip blocks until the first JPEG header appears
    }

    // Clean up open files
    if (out != NULL)                                    // if the last JPEG was open
    {
        fclose(out);                                    // close it
    }
    fclose(in);                                         // close the raw input

    return 0;                                           // success
}
