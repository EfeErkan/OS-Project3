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

void memory_management_unit(struct page_table_entry *page_table, int algorithm, struct frame_table_entry *frame_table, int num_of_frames,
                            struct circular_queue *queue, char address[HEX_LENGTH], FILE *fp_out);

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
                memory_management_unit(page_table, algorithm, frame_table, num_of_frames, &queue, address, fp_out);
            }
        }
        else if (vmmode == 1)
        {
            // Random Address Generation
        }
        
    }

    return 0;
}

void memory_management_unit(struct page_table1_entry *page_table, int algorithm, struct frame_table_entry *frame_table, int num_of_frames,
                            struct circular_queue *queue, char address[HEX_LENGTH], FILE *fp_out)
{
    int page1_index = get_page_part1(address);

    if ( page_table[page1_index].available == USED )
    {
        int page2_index = get_page_part2(address);
                    
        char page_fault = page_table[page1_index].second_level[page2_index].validity;

        if (page_fault == INVALID) // Page Fault
        {
            int frame_index = available_frame(frame_table, num_of_frames);

            if ( frame_index == -1 ) // Page Replacement policy
            {
                if ( algorithm == LRU )
                {

                }
                else if ( algorithm == FIFO )
                {

                }

                }
            else
            {

            }
        }
        else
        {

        }
    }
    else // Exception
    {
        fprintf(fp_out, "%s e\n", address);
    }
}
