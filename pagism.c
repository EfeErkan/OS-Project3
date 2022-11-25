#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "page_table.h"

#define MIN_CMD_LENGTH 7
#define MAX_CMD_LENGTH 9
#define STR_LEN 64
#define LRU 1
#define FIFO 2
#define BASE_ADDRESS "0x00000000"

int main(int argc, char const *argv[])
{
    char in1[STR_LEN];
    char in2[STR_LEN];
    char out[STR_LEN];
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
        num_of_frames = atoi(argv[3]);
        strcpy(out, argv[4]); 
        algorithm = atoi(argv[6]);

        if ( argc == MAX_CMD_LENGTH )
        {
            strcpy(vmsize, argv[8]);
            vmmode = 1;
        }
        else
        {
            fprintf(stderr, "Too many command line arguments!\n");
            exit(-1);
        }

        struct page_table1_entry *page_table = init_page_table();

        FILE *fp_intervals = fopen(in1, "r");
        FILE *fp_addresses = fopen(in2, "r");
        FILE *fp_out = fopen(out, "w");

        if (vmmode == 0)
        {
            char address1[HEX_LENGTH];
            char address2[HEX_LENGTH];

            while ( fscanf(fp_intervals, "%s", address1) != EOF )
            {
                fscanf(fp_intervals, "%s", address2);
                set_page_table_interval(page_table, address1, address2);
            }
        }
        else if (vmmode == 1)
        {
            set_page_table_interval(page_table, BASE_ADDRESS, vmsize);
        }
        
    }

    return 0;
}
