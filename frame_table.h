#ifndef FRAME_TABLE
#define FRAME_TABLE

#define EXISTS '1'
#define EMPTY '0'

// Definitions

struct frame_table_entry
{
    char available;
    int LRU_count;
    int page_number;
};

struct frame_table_entry *init_frame_table(int size);

void free_frame_table(struct frame_table_entry *frame_table);

int available_frame(struct frame_table_entry *frame_table, int size);

// Implementation

struct frame_table_entry *init_frame_table(int size)
{
    struct frame_table_entry *frame_table = malloc(sizeof(struct frame_table_entry) * size);

    for (int i = 0; i < size; i++)
    {
        frame_table[i].available = EMPTY;
        frame_table[i].LRU_count = 0;
    }
}

void free_frame_table(struct frame_table_entry *frame_table)
{
    free(frame_table);
}

int available_frame(struct frame_table_entry *frame_table, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (frame_table[i].available == EMPTY)
        {
            return i;
        }
    }
    return -1;
}

#endif