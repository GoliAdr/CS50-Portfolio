#include <cs50.h>   //get_int()
#include <stdio.h>  //printf()

int calculate_coins(int cents);

int main(void)
{
    // Prompt for the User input
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    // Calculate and print result
    int coins = calculate_coins(cents);
    printf("%i\n", coins);
}

int calculate_coins(int cents)
{
    int coins = 0; // takes as many coins as possible and goes into the next one

    // Quarters
    coins += cents / 25;
    cents %= 25;

    // Dimes
    coins += cents / 10;
    cents %= 10;

    // Nickels
    coins += cents / 5;
    cents %= 5;

    // Pennies
    coins += cents;

    return coins; //returns the coins count to main
}
