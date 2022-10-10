#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int count_digits();
int one_digit();
int two_digits();
bool checksum();
void print_logo();

int main(void)
{
    print_logo();
    printf("Welcome to my Card Verification Program!\n\n");
    long number = get_long("Enter Card Number: ");
    if (number < 4000000000000 || number > 5599999999999999)
    {
        printf("INVALID\n");
        return 0;
    }

    // Count the digits of the card
    int digits = count_digits(number);

    // !Check if 13 digits
    if (digits == 13)
    {
        // Check for Visa
        if (one_digit(number) == 4)
        {
            // Check checksum
            bool visa = checksum(number, digits);
            if (visa)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    // !Check if 15 digits
    else if (digits == 15)
    {
        if (two_digits(number) == 34 || two_digits(number) == 37)
        {
            // Check checksum
            bool AMEX = checksum(number, digits);
            if (AMEX)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    // !Check if 16 digits
    else if (digits == 16)
    {
        // Check for Visa
        if (one_digit(number) == 4)
        {
            // Check checksum
            bool visa = checksum(number, digits);
            if (visa)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        // Check of Master Card
        else if (two_digits(number) > 50 && two_digits(number) < 56)
        {
            // Check checksum
            bool mc = checksum(number, digits);
            if (mc)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    // !Check for everything else
    else
    {
        printf("INVALID\n");
    }
}

int count_digits(long n)
{
    // variable declaration
    int count = 0;
    while (n != 0)
    {
        n = n / 10;
        count++;
    }
    return count;
}

int one_digit(long n)
{
    // variable declaration
    int number;
    do
    {
        n = n / 10;
        number = n;
    }
    while (n > 10);
    return number;
}

int two_digits(long n)
{
    // variable declaration
    int number;
    do
    {
        n = n / 10;
        number = n;
    }
    while (n > 100);
    return number;
}

bool checksum(long n, int digits)
{
    // Extract the digits of the number
    int arr[digits];
    int copy = digits;
    int checksum = 0, checksum2 = 0, count = 0, final_checksum = 0;
    // Extract every digit and stores them in an array
    while (n != 0)
    {
        int digit = n % 10;
        n = n / 10;
        arr[digits - 1] = digit;
        digits = digits - 1;
        count++;
    }
    int digit = copy - 2; // Digit here is copy - 1
    // Calculate every other digit (checksum1)
    while (digit >= 0)
    {
        int first_digit = (arr[digit] * 2) / 10;
        int sec_digit = (arr[digit] * 2) % 10;
        checksum = checksum + first_digit + sec_digit;
        digit -= 2;
    }
    digit = copy - 1; // Digit here should be copy - 1
    // Calculate checksum2
    while (digit >= 0)
    {
        int first_digit = (arr[digit]) / 10;
        int sec_digit = (arr[digit]) % 10;
        checksum2 = checksum2 + first_digit + sec_digit;
        digit -= 2;
    }
    final_checksum = checksum + checksum2;
    if ((final_checksum % 10) == 0)
    {
        return true;
    }
    return false;
}

void print_logo(void){
    printf("░█████╗░██████╗░███████╗██████╗░██╗████████╗  ░█████╗░░█████╗░██████╗░██████╗░\n");
    printf("██╔══██╗██╔══██╗██╔════╝██╔══██╗██║╚══██╔══╝  ██╔══██╗██╔══██╗██╔══██╗██╔══██╗\n");
    printf("██║░░╚═╝██████╔╝█████╗░░██║░░██║██║░░░██║░░░  ██║░░╚═╝███████║██████╔╝██║░░██║\n");
    printf("██║░░██╗██╔══██╗██╔══╝░░██║░░██║██║░░░██║░░░  ██║░░██╗██╔══██║██╔══██╗██║░░██║\n");
    printf("╚█████╔╝██║░░██║███████╗██████╔╝██║░░░██║░░░  ╚█████╔╝██║░░██║██║░░██║██████╔╝\n");
    printf("░╚════╝░╚═╝░░╚═╝╚══════╝╚═════╝░╚═╝░░░╚═╝░░░  ░╚════╝░╚═╝░░╚═╝╚═╝░░╚═╝╚═════╝░\n");
    printf("\n");
    printf("██╗░░░██╗███████╗██████╗░██╗███████╗██╗░█████╗░░█████╗░████████╗██╗░█████╗░███╗░░██╗\n");
    printf("██║░░░██║██╔════╝██╔══██╗██║██╔════╝██║██╔══██╗██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║\n");
    printf("╚██╗░██╔╝█████╗░░██████╔╝██║█████╗░░██║██║░░╚═╝███████║░░░██║░░░██║██║░░██║██╔██╗██║\n");
    printf("░╚████╔╝░██╔══╝░░██╔══██╗██║██╔══╝░░██║██║░░██╗██╔══██║░░░██║░░░██║██║░░██║██║╚████║\n");
    printf("░░╚██╔╝░░███████╗██║░░██║██║██║░░░░░██║╚█████╔╝██║░░██║░░░██║░░░██║╚█████╔╝██║░╚███║\n");
    printf("░░░╚═╝░░░╚══════╝╚═╝░░╚═╝╚═╝╚═╝░░░░░╚═╝░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝\n");
    printf("\n");
    printf("\n");
}
