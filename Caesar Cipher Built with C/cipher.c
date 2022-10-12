#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_logo(void);
void print_plaintext(void);
void print_ciphertext(void);
bool only_digits(string s);

int main(int argc, string argv[])
{
    // Check if there is one CLI argument
    if (argc != 2)
    {
        printf("Usage: ./caesar input\n");
        return 1;
    }

    // Check whether the argument is a number
    bool status = only_digits(argv[1]);
    if (!status)
    {
        printf("Usage: ./caesar input\n");
        return 1;
    }

    // Convert argv[1] from a string to an int
    int key = atoi(argv[1]);

    // Prompt user for a plaintext
    print_logo();
    printf("Welcome to my Caesar Cipher built with C\n\n");
    string plaintext = get_string("Enter plaintext to encode: ");
    printf("\n\n---------------------------------ENCODING...................---------------------------------\n");
    string orig_text = malloc(strlen(plaintext) + 1);
    strcpy(orig_text, plaintext);


    // Iterate through every character in the plaintext
    int len = strlen(plaintext);
    string ciphertext = plaintext;
    for (int i = 0; i < len; i++)
    {
        // If it's uppercase
        if (isupper(plaintext[i]))
        {
            ciphertext[i] = (plaintext[i] + key - 65) % 26 + 65;
        }
        if (islower(plaintext[i]))
        {
            ciphertext[i] = (plaintext[i] + key - 97) % 26 + 97;
        }
    }

    // Print out the ciphertext
    print_plaintext();
    printf("%s\n\n", orig_text);
    print_ciphertext();
    printf("%s\n\n", ciphertext);
}

bool only_digits(string s)
{
    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        if (isdigit(s[i]))
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void print_logo(void){
    printf("░█████╗░░█████╗░███████╗░██████╗░█████╗░██████╗░  ░█████╗░██╗██████╗░██╗░░██╗███████╗██████╗░\n");
    printf("██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗██╔══██╗  ██╔══██╗██║██╔══██╗██║░░██║██╔════╝██╔══██╗\n");
    printf("██║░░╚═╝███████║█████╗░░╚█████╗░███████║██████╔╝  ██║░░╚═╝██║██████╔╝███████║█████╗░░██████╔╝\n");
    printf("██║░░██╗██╔══██║██╔══╝░░░╚═══██╗██╔══██║██╔══██╗  ██║░░██╗██║██╔═══╝░██╔══██║██╔══╝░░██╔══██╗\n");
    printf("╚█████╔╝██║░░██║███████╗██████╔╝██║░░██║██║░░██║  ╚█████╔╝██║██║░░░░░██║░░██║███████╗██║░░██║\n");
    printf("░╚════╝░╚═╝░░╚═╝╚══════╝╚═════╝░╚═╝░░╚═╝╚═╝░░╚═╝  ░╚════╝░╚═╝╚═╝░░░░░╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝\n\n");
}

void print_plaintext(void){
    printf("\n\n█▀█ █░░ ▄▀█ █ █▄░█ ▀█▀ █▀▀ ▀▄▀ ▀█▀ ▀\n");
    printf("█▀▀ █▄▄ █▀█ █ █░▀█ ░█░ ██▄ █░█ ░█░ ▄  ");
}

void print_ciphertext(void){
    printf("\n\n█▀▀ █ █▀█ █░█ █▀▀ █▀█ ▀█▀ █▀▀ ▀▄▀ ▀█▀ ▀\n");
    printf("█▄▄ █ █▀▀ █▀█ ██▄ █▀▄ ░█░ ██▄ █░█ ░█░ ▄  ");
}
