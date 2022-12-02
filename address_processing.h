#ifndef ADDRESS_PROCESSING
#define ADDRESS_PROCESSING

#include <stdlib.h>
#include <string.h>

#define HEX_LENGTH 10
#define HEX_OFFSET_LENGTH 3
#define HEX_PAGE_LENGTH 5
#define ADDRESS_LENGTH 32
#define PAGE_PART1_LENGTH 10
#define PAGE_PART2_LENGTH 10
#define OFFSET_LENGTH 12

// Definitions
char *hex_to_binary(char hex[HEX_LENGTH]);

char *binary_to_hex(char *binary, int binary_length);

int binary_to_decimal(char binary[], int len, int i);

char *decimal_to_binary(int decimal, int binary_length);

void substring(char s[], char sub[], int position, int length);

int get_page_part1(char hex[HEX_LENGTH]);

int get_page_part2(char hex[HEX_LENGTH]);

char *random_address_generation(char base[HEX_LENGTH], char size[HEX_LENGTH]);

// Implementation

char *hex_to_binary(char hex[HEX_LENGTH])
{
    char *binary = (char*) malloc(ADDRESS_LENGTH + 1);
    binary[0] = '\0';

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

char *binary_to_hex(char *binary, int binary_length)
{
    char *hex = malloc(binary_length / 4 + 2);

    hex[0] = '0'; hex[1] = 'x';

    char segment[4];
    char c;

    for (int i = 0; i < binary_length / 4; i++)
    {
        substring(binary, segment, i * 4, 4);

        if (strcmp(segment, "0000") == 0)
            c = '0';
        else if (strcmp(segment, "0001") == 0)
            c = '1';
        else if (strcmp(segment, "0010") == 0)
            c = '2';
        else if (strcmp(segment, "0011") == 0)
            c = '3';
        else if (strcmp(segment, "0100") == 0)
            c = '4';
        else if (strcmp(segment, "0101") == 0)
            c = '5';
        else if (strcmp(segment, "0110") == 0)
            c = '6';
        else if (strcmp(segment, "0111") == 0)
            c = '7';
        else if (strcmp(segment, "1000") == 0)
            c = '8';
        else if (strcmp(segment, "1001") == 0)
            c = '9';
        else if (strcmp(segment, "1010") == 0)
            c = 'a';
        else if (strcmp(segment, "1011") == 0)
            c = 'b';
        else if (strcmp(segment, "1100") == 0)
            c = 'c';
        else if (strcmp(segment, "1101") == 0)
            c = 'd';
        else if (strcmp(segment, "1110") == 0)
            c = 'e';
        else if (strcmp(segment, "1111") == 0)
            c = 'f';

        hex[i + 2] = c;
    }
    return hex;
}

int binary_to_decimal(char binary[], int len, int i)
{
   if (i == len - 1)
        return (binary[i] - '0');

   int temp = binary[i] - '0';

   temp = temp << (len - i - 1);
   temp = temp + binary_to_decimal(binary, len, i + 1);
   return temp;
}

char *decimal_to_binary(int decimal, int binary_length)
{
    int binaryNum[32];

    char *binary = malloc(binary_length);
  
    // counter for binary array
    int i = 0;
    while (decimal > 0) 
    {
        binaryNum[i] = decimal % 2;
        decimal = decimal / 2;
        i++;
    }

    for (int j = binary_length - 1; j >= 0; j--)
    {
        if (j >= i)
        {
            binary[binary_length - j - 1] = '0';
        }
        else
        {
            binary[binary_length - j - 1] = binaryNum[j] + '0';
        }
        
    }
    return binary;   
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

    char *binary = hex_to_binary(hex);
    substring(binary, page_part1_binary, 0, PAGE_PART1_LENGTH);

    free(binary);

    return binary_to_decimal(page_part1_binary, PAGE_PART1_LENGTH, 0);
}

int get_page_part2(char hex[HEX_LENGTH])
{
    char page_part2_binary[PAGE_PART2_LENGTH];

    char *binary = hex_to_binary(hex);
    substring(binary, page_part2_binary, PAGE_PART1_LENGTH, PAGE_PART2_LENGTH);

    free(binary);
    
    return binary_to_decimal(page_part2_binary, PAGE_PART2_LENGTH, 0);
}

char *random_address_generation(char base[HEX_LENGTH], char size[HEX_LENGTH])
{
    char *base_binary = hex_to_binary(base);
    int base_int = binary_to_decimal(base_binary, ADDRESS_LENGTH, 0);
    free(base_binary);

    char *size_binary = hex_to_binary(size);
    int size_int = binary_to_decimal(size_binary, ADDRESS_LENGTH, 0);
    free(size_binary);

    int random_address_int = ( rand() % size_int ) + base_int;
    char *random_address_binary = decimal_to_binary(random_address_int, ADDRESS_LENGTH);
    char *random_address_hex = binary_to_hex(random_address_binary, ADDRESS_LENGTH);
    free(random_address_binary);

    return random_address_hex;
}

#endif