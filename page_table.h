#ifndef PAGE_TABLE
#define PAGE_TABLE

#include <stdlib.h>
#include "address_processing.h"

#define PAGE_TABLE1_LEN 1024
#define PAGE_TABLE2_LEN 1024

#define USED '1'
#define UNUSED '0'
#define VALID '1'
#define INVALID '0'

//Definitions
struct page_table1_entry
{
    char available;
    struct page_table2_entry *second_level;
};

struct page_table2_entry
{
    char validity;
    int frame_number;
};

struct page_table1_entry *init_page_table();

void free_page_table(struct page_table1_entry *page_table);

void set_page_table(struct page_table1_entry *page_table, char hex1[HEX_LENGTH], char hex2[HEX_LENGTH]);

// Implementation

struct page_table1_entry *init_page_table()
{
    struct page_table1_entry *page_table = malloc(sizeof(struct page_table1_entry) * PAGE_TABLE1_LEN);

    for (int i = 0; i < PAGE_TABLE1_LEN; i++)
    {
        page_table[i].available = UNUSED;
        page_table[i].second_level = NULL;
    }
    return page_table;
}

void free_page_table(struct page_table1_entry *page_table)
{
    for (int i = 0; i < PAGE_TABLE1_LEN; i++)
    {
        if (page_table[i].available == USED)
        {
            free(page_table[i].second_level);
        }
    }
    free(page_table);
}

void set_page_table_interval(struct page_table1_entry *page_table, char hex1[HEX_LENGTH], char hex2[HEX_LENGTH])
{
    int start = get_page_part1(hex1);
    int end = get_page_part1(hex2);

    for (int i = start; i <= end; i++)
    {
        page_table[i].available = USED;
        page_table[i].second_level = malloc(sizeof(struct page_table2_entry) * PAGE_TABLE2_LEN);

        for (int j = 0; j < PAGE_TABLE2_LEN; j++)
        {
            page_table[i].second_level[j].validity = INVALID;
        }
    }
}

#endif