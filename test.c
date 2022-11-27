#include <stdio.h>
#include <stdlib.h>

#include "address_processing.h"

int main(int argc, char const *argv[])
{
    int decimal = 10;

    char *binary = decimal_to_binary(decimal, 20);

    for (int i = 0; i < 20; i++)
    {
        printf("%c", binary[i]);
    }
    printf("\n");

    free(binary);

    return 0;
}
