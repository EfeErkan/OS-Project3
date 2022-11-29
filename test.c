#include <stdio.h>
#include <stdlib.h>

#include "address_processing.h"

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 10; i++)
    {
        char *address = random_address_generation("0x00000000", "0x10000000");

        for (int i = 0; i < 10; i++)
        {
            printf("%c", address[i]);
        }
        printf("\n");

        free(address);
    }
    
    return 0;
}
