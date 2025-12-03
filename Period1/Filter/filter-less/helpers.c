#include "helpers.h"          // RGBTRIPLE type + function declarations
#include <math.h>             // roundf()

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) // turn every pixel grey
{
    for (int i = 0; i < height; i++)             // every row
    {
        for (int j = 0; j < width; j++)          // every column
        {
            float avg = (image[i][j].rgbtRed
                        + image[i][j].rgbtGreen
                        + image[i][j].rgbtBlue) / 3.0f;           // average RGB
            BYTE g = (BYTE) roundf(avg);                          // round to whole 0..255

            image[i][j].rgbtRed   = g;     // set R to grey
            image[i][j].rgbtGreen = g;     // set G to grey
            image[i][j].rgbtBlue  = g;     // set B to grey
        }
    }
}

// Clamp helper: keep value within 0..255
static inline BYTE clamp_to_byte(int v)  // below 0 → 0, above 255 → 255
{
    if (v < 0)   return 0;               // too small → black
    if (v > 255) return 255;             // too big → white
    return (BYTE) v;                     // otherwise ok
}

// Convert image to sepia (vintage brown)
void sepia(int height, int width, RGBTRIPLE image[height][width]) // apply sepia formula
{
    for (int i = 0; i < height; i++)                   // each row
    {
        for (int j = 0; j < width; j++)                // each column
        {
            int r = image[i][j].rgbtRed;               // original R
            int g = image[i][j].rgbtGreen;             // original G
            int b = image[i][j].rgbtBlue;              // original B

            int sr = (int) roundf(0.393f*r + 0.769f*g + 0.189f*b); // sepia R
            int sg = (int) roundf(0.349f*r + 0.686f*g + 0.168f*b); // sepia G
            int sb = (int) roundf(0.272f*r + 0.534f*g + 0.131f*b); // sepia B

            image[i][j].rgbtRed   = clamp_to_byte(sr); // cap to 0..255
            image[i][j].rgbtGreen = clamp_to_byte(sg); // cap to 0..255
            image[i][j].rgbtBlue  = clamp_to_byte(sb); // cap to 0..255
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) // mirror left↔right
{
    for (int i = 0; i < height; i++)                 // each row
    {
        for (int j = 0; j < width / 2; j++)          // swap until the middle
        {
            RGBTRIPLE tmp = image[i][j];                          // save left
            image[i][j] = image[i][width - 1 - j];                // right → left
            image[i][width - 1 - j] = tmp;                        // saved left → right
        }
    }
}

// Blur image with 3x3 box
void blur(int height, int width, RGBTRIPLE image[height][width]) // average neighbors
{
    RGBTRIPLE copy[height][width];                // read from copy, write to image

    for (int i = 0; i < height; i++)             // copy rows
    {
        for (int j = 0; j < width; j++)          // copy cols
        {
            copy[i][j] = image[i][j];            // exact copy
        }
    }

    for (int i = 0; i < height; i++)             // for each pixel
    {
        for (int j = 0; j < width; j++)
        {
            int sumR = 0, sumG = 0, sumB = 0;    // totals
            int cnt = 0;                         // how many pixels added

            for (int di = -1; di <= 1; di++)     // neighbor rows (-1..1)
            {
                for (int dj = -1; dj <= 1; dj++) // neighbor cols (-1..1)
                {
                    int ni = i + di;             // neighbor row index
                    int nj = j + dj;             // neighbor col index

                    if (ni < 0 || ni >= height || nj < 0 || nj >= width)
                        continue;                // skip out-of-bounds

                    sumR += copy[ni][nj].rgbtRed;    // add R
                    sumG += copy[ni][nj].rgbtGreen;  // add G
                    sumB += copy[ni][nj].rgbtBlue;   // add B
                    cnt++;                           // count one neighbor
                }
            }

            image[i][j].rgbtRed   = (BYTE) roundf((float) sumR / cnt); // avg R
            image[i][j].rgbtGreen = (BYTE) roundf((float) sumG / cnt); // avg G
            image[i][j].rgbtBlue  = (BYTE) roundf((float) sumB / cnt); // avg B
        }
    }
}
