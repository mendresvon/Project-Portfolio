#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int count_letters(string input);
int count_words(string input);
int count_sentences(string input);

int main(void)
{
    // Get a text input from the user
    string input = get_string("Text: ");

    // Count the letters, words, and sentences of the inputted file
    int letters = count_letters(input);
    int words = count_words(input);
    int sentences = count_sentences(input);

    // Calculate the grade index of the inputted text
    int index = round(0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8);
}

int count_letters(string input)
{
    int letters = 0;
    // Iterate through every character
    for (int i = 0; i < strlen(input); i++)
    {
        if (isalpha(input[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string input)
{
    int words = 1;
    // Iterate through every letter
    for (int i = 0; i < strlen(input); i++)
    {
        char ch = char(input[i]);
        if (strcmp(ch, " ") == 0)
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string input)
{
    int sentences = 0;
    // Iterate through every character
    for (int i = 0; i < strlen(input); i++)
    {
        char ch = char(input[i]);
        if (strcmp(ch, ".") == 0 || strcmp(ch, "?") == 0 || strcmp(ch, "!") == 0)
        {
            sentences++;
        }
    }
    return sentences;
}