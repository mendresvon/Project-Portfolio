#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height, row, column, space;

    //prompts the user for a height between 1 to 8
    do
    {
         height = get_int("height: \n");
    }
    while (height < 1 || height > 8);

    //build the pyramid
    for (row = 0; row < height; row++)
    {
        //places the spaces before the hash
        for (space = 0; space < height - row - 1; space++)
        {
            printf(" ");
        }

        //places the hash after the spaces
        for (column = 0; column < row + 1; column++)
        {
            printf("#");
        }

        //places the space between the pyramid
        printf("  ");

        //builds the right side of the pyramind
        for (column = 0; column < row + 1; column++)
        {
        printf("#");
        }
        printf("\n");
    }
}