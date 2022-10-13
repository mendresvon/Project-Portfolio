#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char lowercase[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
char uppercase[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

bool is_alphabetical(string key);
bool is_duplicate(string key);

int main(int argc, string argv[])
{
    // Check for correct command-line argument
    if (argc != 2 || strlen(argv[1]) != 26)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Check if key is alphabetical
    string key = argv[1];
    bool alpha = is_alphabetical(key);
    bool dupl = is_duplicate(key);
    if (!alpha)
    {
        printf("Error: Non-alphabetical input\n");
        return 1;
    }
    if (!dupl)
    {
        printf("Error: Duplicated character in key\n");
        return 1;
    }
    // Check for duplicate characters in the key

    // Ask the user for plaintext
    string plaintext = get_string("plaintext: ");
    int len = strlen(plaintext);

    // Iterate through every character of the plaintext
    for (int i = 0; i < len; i++)
    {
        // Check if the current character is uppercase
        if (isupper(plaintext[i]))
        {
            // Compare the current char in plaintext with the original alphabet and determine its place in the alphabet
            for (int j = 0; j < 26; j++)
            {
                // j is the place of the ith character in the alphabet
                if (plaintext[i] == uppercase[j])
                {
                    // Subtitute plaintext with the jth element of the key
                    plaintext[i] = toupper(key[j]);
                    break;
                }
            }

        }
        // Check if the current char is lowercase
        else if (islower(plaintext[i]))
        {
            // Compare the current char in plaintext with the original alphabet and determine its place in the alphabet
            for (int j = 0; j < 26; j++)
            {
                if (plaintext[i] == lowercase[j])
                {
                    plaintext[i] = tolower(key[j]);
                    break;
                }
            }
        }
    }
    printf("ciphertext: %s\n", plaintext);

}

bool is_alphabetical(string s)
{
    // Iterate through every character of s
    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        if (!isalpha(s[i]))
        {
            return false;
        }
    }
    return true;
}
bool is_duplicate(string s)
{
    // Initialize status to true
    bool status = true;
    int count = 0;
    string copy = s;
    // Copy every character of s into copy one char at a time
    for (int i = 0; i < strlen(s); i++)
    {
        for (int j = 0; j < strlen(s); j++)
        {
            // Scan if ith char of s already exists in copy; if it doesn't copy the char
            if (s[i] == copy[j])
            {
                count++;
            }
            if (count > 1)
            {
                status = false;
                break;
            }
        }
        count = 0;
    }
    return status;
}