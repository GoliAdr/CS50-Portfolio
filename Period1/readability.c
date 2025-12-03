#include <ctype.h>      //isalpha
#include <cs50.h>       //string, get_string
#include <math.h>       //round()
#include <stdio.h>      //printf()
#include <string.h>     //strlen()
//Headers that are neded for the code


int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
// Helper funtions so main knows what to do

int main(void)
{
    // Ask the User for the text
    string text = get_string("Text: ");

    // Count the symbols word etc in a text
    int letters   = count_letters(text);
    int words     = count_words(text);
    int sentences = count_sentences(text);

    // Coleman - Liau index
    double L = (double) letters * 100.0 / (double) words;
    double S = (double) sentences * 100.0 / (double) words;
    int grade = (int) round(0.0588 * L - 0.296 * S - 15.8); //round to nearest integer and store as int

    //Print the Level of the Grade
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text) //count the letters and return the Total amount
{
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha((unsigned char) text[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text) // count the words and return total
{
    int words = 1; // words are separated by spaces
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text) // counts . ! ? and returns total
{
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}
