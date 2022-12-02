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
    char available;
    char validity;
    int frame_number;
};

struct page_table1_entry *init_page_table();

void free_page_table(struct page_table1_entry *page_table);

void set_page_table_interval(struct page_table1_entry *page_table, char hex1[HEX_LENGTH], char hex2[HEX_LENGTH]);

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
    int hex1_page1_num = get_page_part1(hex1);
    int hex1_page2_num = get_page_part2(hex1);
    int hex2_page1_num = get_page_part1(hex2);
    int hex2_page2_num = get_page_part2(hex2);

    printf("%d %d -- %d %d\n", hex1_page1_num, hex1_page2_num, hex2_page1_num, hex2_page2_num);

    for (int i = hex1_page1_num; i <= hex2_page1_num; i++)
    {
        if (page_table[i].available == UNUSED)
        {
            page_table[i].available = USED;
            page_table[i].second_level = malloc(sizeof(struct page_table2_entry) * PAGE_TABLE2_LEN);

            for (int j = 0; j < PAGE_TABLE2_LEN; j++)
            {
                page_table[i].second_level[j].available = UNUSED;
            }
        }
    }

    int p1 = hex1_page1_num;
    int p2 = hex1_page2_num;

    while (p1 != hex2_page1_num || p2 != hex2_page2_num)
    {
        page_table[p1].second_level[p2].available = USED;
        page_table[p1].second_level[p2].validity = INVALID;

        p2++;

        if (p2 == PAGE_TABLE2_LEN)
        {
            p2 = 0;
            p1++;
        }
    }
}

#endif