#include <stdio.h>
#include <stdlib.h>

#include "address_processing.h"

#define MIN_CMD_LENGTH 7
#define MAX_CMD_LENGTH 9
#define STR_LEN 64
#define LRU 1
#define FIFO 2

int main(int argc, char const *argv[])
{
    char in1[STR_LEN];
    char in2[STR_LEN];
    int num_of_frames;
    int algorithm;
    char vmsize[HEX_LENGTH];
    int vmmode = 0;

    if ( argc < MIN_CMD_LENGTH )
    {
        fprintf(stderr, "Command line arguments are insufficient!\n");
        exit(-1);
    }
    else
    {
        strcpy(in1, argv[1]); strcpy(in2, argv[2]);
        num_of_frames = atoi(argv[3]); algorithm = atoi(argv[5]);

        if ( argc == MAX_CMD_LENGTH )
        {
            strcpy(vmsize, argv[9]);
            vmmode = 1;
        }
        else
        {
            fprintf(stderr, "Too many command line arguments!\n");
            exit(-1);
        }

        
    }

    return 0;
}
