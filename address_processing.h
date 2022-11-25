#ifndef ADDRESS_PROCESSING
#define ADDRESS_PROCESSING

#include <stdlib.h>
#include <string.h>

#define HEX_LENGTH 10
#define ADDRESS_LENGTH 32
#define PAGE_PART1_LENGTH 10
#define PAGE_PART2_LENGTH 10
#define OFFSET_LENGTH 12

// Definitions
char *hex_to_binary(char hex[HEX_LENGTH]);

int binary_to_decimal(char binary[], int len, int i);

void substring(char s[], char sub[], int position, int length);

int get_page_part1(char hex[HEX_LENGTH]);

int get_page_part2(char hex[HEX_LENGTH]);


// Implementation

char *hex_to_binary(char hex[HEX_LENGTH])
{
    char *binary = (char*) malloc(ADDRESS_LENGTH);

    for ( int i = 2; i < HEX_LENGTH; i++ ) 
    {
        char segment[5];
        switch (hex[i]) 
        {
            case '0':
                strcpy(segment, "0000");
                break;
            case '1':
                strcpy(segment, "0001");
                break;
            case '2':
                strcpy(segment, "0010");
                break;
            case '3':
                strcpy(segment, "0011");
                break;
            case '4':
                strcpy(segment, "0100");
                break;
            case '5':
                strcpy(segment, "0101");
                break;
            case '6':
                strcpy(segment, "0110");
                break;
            case '7':
                strcpy(segment, "0111");
                break;
            case '8':
                strcpy(segment, "1000");
                break;
            case '9':
                strcpy(segment, "1001");
                break;
            case 'a':
                strcpy(segment, "1010");
                break;
            case 'b':
                strcpy(segment, "1011");
                break;
            case 'c':
                strcpy(segment, "1100");
                break;
            case 'd':
                strcpy(segment, "1101");
                break;
            case 'e':
                strcpy(segment, "1110");
                break;
            case 'f':
                strcpy(segment, "1111");
                break;
        }
        strcat(binary, segment);
    }
    return binary;
}

int binary_to_decimal(char binary[], int len, int i)
{
   if (i == len - 1)
        return (binary[i] - '0');

   int temp = binary[i] - '0';

   temp = temp << len - i - 1;
   temp = temp + binary_to_decimal(binary, len, i + 1);
   return temp;
}

void substring(char s[], char sub[], int position, int length) 
{
   int c = 0;
   
   while (c < length) 
   {
      sub[c] = s[position + c];
      c++;
   }
   sub[c] = '\0';
}

int get_page_part1(char hex[HEX_LENGTH])
{
    char page_part1_binary[PAGE_PART1_LENGTH];
    int page_num;

    char *binary = hex_to_binary(hex);
    substring(binary, page_part1_binary, 0, PAGE_PART1_LENGTH);

    free(binary);

    return binary_to_decimal(page_part1_binary, PAGE_PART1_LENGTH, 0);
}

int get_page_part2(char hex[HEX_LENGTH])
{
    char page_part2_binary[PAGE_PART2_LENGTH];
    int page_num;

    char *binary = hex_to_binary(hex);
    substring(binary, page_part2_binary, PAGE_PART1_LENGTH, PAGE_PART2_LENGTH);

    free(binary);
    
    return binary_to_decimal(page_part2_binary, PAGE_PART2_LENGTH, 0);
}

#endif