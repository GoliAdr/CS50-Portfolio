#include <stdio.h>    // printf()
#include <cs50.h>     // get_int()


void print_row(int spaces, int bricks);

int main(void)
{
    // prompt the user for the pyramid's height
    int n;
    do
    {
        n = get_int("height: ");
    }
    while (n < 1 || n > 8);
    // print a pyramid of that height
    for (int i = 0; i < n; i++)
    {
    int bricks = i + 1;
    int spaces = n - i - 1;

    // Print row of bricks
    print_row(spaces, bricks);
    }
}
void print_row(int spaces, int bricks)
{
    for (int i = 0; i < spaces ; i++)
    {
        printf(" ");
    }
    for (int i = 0; i < bricks ; i++)
    {
        printf("#");
    }
    printf("\n");
    //print row of bricks
}
