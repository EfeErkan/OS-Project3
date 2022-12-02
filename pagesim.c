#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "page_table.h"
#include "circular_queue.h"
#include "frame_table.h"

#define MIN_CMD_LENGTH 7
#define MAX_CMD_LENGTH 9
#define STR_LEN 64
#define LRU 1
#define FIFO 2
#define BASE_ADDRESS "0x00000000"

void memory_management_unit(struct page_table1_entry *page_table, int algorithm, struct frame_table_entry *frame_table, int num_of_frames,
                            struct circular_queue *queue, char address[HEX_LENGTH], FILE *fp_out);

int main(int argc, char const *argv[])
{
    char in1[STR_LEN];
    char in2[STR_LEN];
    char out[STR_LEN];
    int num_of_frames;
    int algorithm;
    char vmsize[HEX_LENGTH];
    int addrcount;
    int vmmode = 0;

    FILE *fp_intervals = NULL;
    FILE *fp_addresses = NULL;

    if ( argc < MIN_CMD_LENGTH || argc > MAX_CMD_LENGTH )
    {
        fprintf(stderr, "Incorrect number of Command line arguments!\n");
        exit(-1);
    }
    else
    {
        if (argc == MIN_CMD_LENGTH)
        {
            strcpy(in1, argv[1]); strcpy(in2, argv[2]);
            num_of_frames = atoi(argv[3]);
            strcpy(out, argv[4]);
            algorithm = atoi(argv[6]);

            fp_intervals = fopen(in1, "r");
            fp_addresses = fopen(in2, "r");
            vmmode = 0;
        }
        else if (argc == MAX_CMD_LENGTH)
        {
            num_of_frames = atoi(argv[1]);
            strcpy(out, argv[2]);
            algorithm = atoi(argv[4]);
            strcpy(vmsize, argv[6]);
            addrcount = atoi(argv[8]);
            vmmode = 1;
        }

        FILE *fp_out = fopen(out, "w");

        struct page_table1_entry *page_table = init_page_table();

        struct frame_table_entry *frame_table = init_frame_table(num_of_frames);

        struct circular_queue queue;
        init_queue(&queue, num_of_frames);

        // Adjusting Page Table
        if (vmmode == 0)
        {
            char address1[HEX_LENGTH];
            char address2[HEX_LENGTH];
            char buffer[HEX_LENGTH];

            int count = 0;

            while ( fscanf(fp_intervals, "%s", buffer) != EOF )
            {
                count++;
                if (count % 2 == 1)
                {
                    strcpy(address1, buffer);
                }
                else
                {
                    strcpy(address2, buffer);
                    set_page_table_interval(page_table, address1, address2);
                }
            }
        }
        else if (vmmode == 1)
        {
            set_page_table_interval(page_table, BASE_ADDRESS, vmsize);
        }

        fprintf(fp_out, "%-10s %-10s\n", "Virtual", "Physical");

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
            for (int i = 0; i < addrcount; i++)
            {
                char *address = random_address_generation(BASE_ADDRESS, vmsize);
                memory_management_unit(page_table, algorithm, frame_table, num_of_frames, &queue, address, fp_out);
                free(address);
            }
        }

        free_page_table(page_table);
        free_frame_table(frame_table);
        free_queue(&queue);

        fclose(fp_out);

        if (vmmode == 0)
        {
            fclose(fp_addresses);
            fclose(fp_intervals);
        }
        
    }

    return 0;
}

void memory_management_unit(struct page_table1_entry *page_table, int algorithm, struct frame_table_entry *frame_table, int num_of_frames,
                            struct circular_queue *queue, char address[HEX_LENGTH], FILE *fp_out)
{
    int page1_index = get_page_part1(address);
    int page2_index = get_page_part2(address);

    if ( page_table[page1_index].available == USED && page_table[page1_index].second_level[page2_index].available == USED )
    {  
        char page_fault = page_table[page1_index].second_level[page2_index].validity;

        if (page_fault == INVALID) // Page Fault
        {
            int frame_index = available_frame(frame_table, num_of_frames);

            if ( frame_index == -1 ) // Page Replacement policy
            {
                if ( algorithm == LRU )
                {
                    frame_index = LRU_frame(frame_table, num_of_frames);
                }
                else if ( algorithm == FIFO )
                {
                    frame_index = dequeue(queue);
                }

                int p1 = frame_table[frame_index].page_number1;
                int p2 = frame_table[frame_index].page_number2;

                page_table[p1].second_level[p2].validity = INVALID;
            }
            
            page_table[page1_index].second_level[page2_index].frame_number = frame_index;
            page_table[page1_index].second_level[page2_index].validity = VALID;
            frame_table[frame_index].available = EXISTS;
            frame_table[frame_index].page_number1 = page1_index;
            frame_table[frame_index].page_number2 = page2_index;
            update_LRU(frame_table, num_of_frames, frame_index);
            
            if (algorithm == FIFO)
                enqueue(queue, frame_index);


            char *binary_frame_number = decimal_to_binary(frame_index, PAGE_PART1_LENGTH + PAGE_PART2_LENGTH);
            char *hex_frame_number = binary_to_hex(binary_frame_number, PAGE_PART1_LENGTH + PAGE_PART2_LENGTH);

            char *offset = malloc(4);
            substring(address, offset, 7, 3);
            
            char physical_address[ADDRESS_LENGTH];
            physical_address[0] = '\0';
            strcat(physical_address, hex_frame_number);
            strcat(physical_address, offset);

            free(offset);
            
            fprintf(fp_out, "%s %s x\n", address, physical_address);

            free(binary_frame_number);
            free(hex_frame_number);
        }
        else // No Page Fault
        {
            int translation_frame = page_table[page1_index].second_level[page2_index].frame_number;
            
            update_LRU(frame_table, num_of_frames, translation_frame);

            if (algorithm == FIFO)
                enqueue(queue, translation_frame);

            char *binary_frame_number = decimal_to_binary(translation_frame, PAGE_PART1_LENGTH + PAGE_PART2_LENGTH);
            char *hex_frame_number = binary_to_hex(binary_frame_number, PAGE_PART1_LENGTH + PAGE_PART2_LENGTH);

            char *offset = malloc(4);
            substring(address, offset, 7, 3);

            char physical_address[ADDRESS_LENGTH];
            physical_address[0] = '\0';
            strcat(physical_address, hex_frame_number);
            strcat(physical_address, offset);

            free(offset);
            
            fprintf(fp_out, "%s %s\n", address, physical_address);

            free(binary_frame_number);
            free(hex_frame_number);
        }
    }
    else // Exception
    {
        fprintf(fp_out, "%s e\n", address);
    }

    //print_LRU_counts(frame_table, num_of_frames);
}
