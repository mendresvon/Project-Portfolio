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
    int index = round((0.0588 * letters / words * 100) - (0.296 * sentences / words * 100) - 15.8);

    // Prints the Grade level index of the input text
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Function for counting letters
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

// Function for counting letters
int count_words(string input)
{
    int words = 1;
    // Iterate through every letter
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == ' ')
        {
            words++;
        }
    }
    return words;
}

// Function for counting letters
int count_sentences(string input)
{
    int sentences = 0;
    // Iterate through every character
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '.' || input[i] == '?' || input[i] == '!')
        {
            sentences++;
        }
    }
    return sentences;
}