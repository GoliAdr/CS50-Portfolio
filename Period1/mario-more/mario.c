#include <stdio.h>
#include <cs50.h>

void print_double_row(int spaces, int bricks);

int main(void)
{
    // prompt the user for the pyramid's height
    int n;
    do
    {
        n = get_int("height: ");
    }
    // set a maximum height so my laptop won't crash
    while (n < 1 || n > 8);
    // print a pyramid of that height
    for (int i = 0; i < n; i++)
    {
    int bricks = i + 1;
    int spaces = n - i - 1;
        // Print row of bricks and mirror it
        print_double_row (spaces, bricks);
    }
}
void print_double_row(int spaces, int bricks)
{
    for (int k = 0; k < spaces; k++)
    {
        printf(" ");
    }
        for (int k = 0; k < bricks; k++)
    {
        printf("#");
    }
    {
        printf("  ");
    }
        for (int k = 0; k < bricks; k++)
    {
        printf("#");
    }
        printf("\n");

}
