#include <cs50.h>      // get_string(), string
#include <stdio.h>     // printf()
#include <ctype.h>     // isupper(), islower()
#include <string.h>    // strlen()


// Points per letter A..Z (A=0 index, B=1, ... Z=25)
int points[] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8,
    5, 1, 3, 1, 1, 3,10, 1, 1, 1,
    1, 4, 4, 8, 4,10
};

int compute_score(string player); // forward declaration

int main(void)
{
    string player1 = get_string("Player 1: ");   // read first word
    string player2 = get_string("Player 2: ");   // read second word

    int score1 = compute_score(player1);         // total for player 1
    int score2 = compute_score(player2);         // total for player 2

    if (score1 > score2)                         // compare and print result
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string player)
{
    int score = 0;                               // running total

    // loop over every character in the input string
    for (int i = 0, len = strlen(player); i < len; i++)
    {
        char c = player[i];                      // current character

        if (isupper(c))                          // uppercase A..Z
        {
            score += points[c - 'A'];            // map 'A'..'Z' to 0..25
        }
        else if (islower(c))                     // lowercase a..z
        {
            score += points[c - 'a'];            // map 'a'..'z' to 0..25
        }
        else
        {
            // non-letters (spaces, punctuation, digits) → worth 0
            // do nothing
        }
    }

    return score;                                // final total
}
