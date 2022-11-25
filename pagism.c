#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "page_table.h"
#include "circular_queue.h"
#include "frame_table.h"

#define CMD_LENGTH 7
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

    FILE *fp_intervals;
    FILE *fp_addresses;
    FILE *fp_out;

    if ( argc != CMD_LENGTH )
    {
        fprintf(stderr, "Incorrect number of Command line arguments!\n");
        exit(-1);
    }
    else
    {
        if (strcmp(argv[5], "-a") == 0)
        {
            strcpy(in1, argv[1]); strcpy(in2, argv[2]);
            num_of_frames = atoi(argv[3]);
            strcpy(out, argv[4]);
            algorithm = atoi(argv[6]);

            fp_intervals = fopen(in1, "r");
            fp_addresses = fopen(in2, "r");
            vmmode = 0;
        }
        else if (strcmp(argv[5], "-r") == 0)
        {
            num_of_frames = atoi(argv[1]);
            strcpy(out, argv[2]);
            algorithm = atoi(argv[4]);
            strcpy(vmsize, argv[6]);
            vmmode = 1;
        }

        FILE *fp_out = fopen(out, "w");

        struct page_table1_entry *page_table = init_page_table();

        struct frame_table_entry *frame_table = init_frame_table(num_of_frames);

        struct circular_queue queue;
        init_queue(queue, num_of_frames);

        // Adjusting Page Table
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

        // Paging
        if (vmmode == 0)
        {
            char address[HEX_LENGTH];

            while ( fscanf(fp_addresses, "%s", address) != EOF )
            {
                int page1_index = get_page_part1(address);
            }
        }
        else if (vmmode == 1)
        {

        }
        
    }

    return 0;
}
