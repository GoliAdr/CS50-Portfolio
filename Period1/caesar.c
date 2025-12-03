#include <cs50.h>       //get_string(), string
#include <ctype.h>      //isdigit(), isupper(), islower()
#include <stdbool.h>    //bool, true, false
#include <stdio.h>      //printf()
#include <stdlib.h>     //atoi()
#include <string.h>     //strlen()

bool only_digits(string s); // true if s is all 0–9
char rotate(char c, int k); // shift a single char by k

int main(int argc, string argv[])
{
    if (argc != 2) // need exactly one arg: the key. ./caesar arg 1 and e.g 5 is arg 2, ./caesar 5
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    if (!only_digits(argv[1])) // key must be numeric, which kind of key is it
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int k = atoi(argv[1]); // change the string to a number. Makes e.g "(c - 'A' + k) % 26" possible

    printf("plaintext:  ");
    string p = get_string(""); // read line from user

    printf("ciphertext: ");
    for (int i = 0, n = strlen(p); i < n; i++)
    {
        printf("%c", rotate(p[i], k)); // encrypt char-by-char
    }
    printf("\n");
    return 0;
}

bool only_digits(string s)
{
    if (s == NULL || s[0] == '\0')
        return false; // empty/NULL -> false
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit((unsigned char) s[i]))
            return false; // any non-digit -> false
    }
    return true; // all digits
}

char rotate(char c, int k)
{
    k %= 26; // keep shift in 0–25
    if (isupper((unsigned char) c))
        return 'A' + ((c - 'A' + k) % 26); // wrap A–Z
    if (islower((unsigned char) c))
        return 'a' + ((c - 'a' + k) % 26); // wrap a–z
    return c;                              // non-letters unchanged
}
