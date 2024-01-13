#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "binary-find-pi.h"

#define BUFFER_SIZE 4096
#define LIMIT 10000000 // limit for digits read

int binary_to_decimal(char bin[])
{
    int dec = 0;
    int mul = 1;
    for (int i = strlen(bin) - 1; i >= 0; --i)
    {
        if (bin[i] != '1' && bin[i] != '0')
            continue;

        if (bin[i] == '1')
            dec += mul;

        mul *= 2;
    }
    return dec;
}

void binary_to_hex(char bin[], char *hex, size_t hex_size)
{
    int sum = 0;
    int pos = 8;
    int digit = 0;

    char hexNums[] = "0123456789abcdef";
    for (int i = 0; i < strlen(bin); ++i)
    {
        if (bin[i] != '1' && bin[i] != '0')
            continue;

        if (bin[i] == '1')
            sum += pos;

        pos /= 2;

        // Store the sum
        if (pos <= 0)
        {
            if (digit < hex_size - 1)
                hex[digit] = hexNums[sum];

            sum = 0;
            pos = 8;
            digit++;
        }
    }
}

unsigned long int find_position_in_pi(char bin[])
{
    char hex[5] = {0};
    binary_to_hex(bin, hex, sizeof(hex));

    // Storage for positioning
    int char_count = 0;
    int char_position = 0;
    unsigned long int file_position = 0;
    bool found = false;

    // Open file and create buffer
    FILE *file;
    char *filename = "pi-billion.txt";
    char buffer[BUFFER_SIZE];

    file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    while (!feof(file) && char_count < LIMIT)
    {
        size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);

        if (bytesRead == 0)
            break;

        // Loop through current buffer
        for (int i = 0; i < BUFFER_SIZE; ++i)
        {
            // Check if the current hex matches the buffer
            if (buffer[i] == hex[char_position])
                char_position++;
            else
                char_position = 0;
                

            // Found the position
            if (char_position >= 4)
            {
                found = true;
                break;
            }

            file_position++;
        }

        if (found)
            break;
        
        char_count += BUFFER_SIZE; // Limits the amount of chars read to LIMIT
    }


    // If we found it, let the user know the position to find
    if (found)
        return file_position - 5;
    else
        printf("Could not find within the LIMIT specified.");

    return 0;
}

/*
int main()
{

    unsigned long int file_position = find_position_in_pi("0111 1100 1111 0101");

    if (file_position != 0)
    {
        printf("Found at hex position: %lu\nFound at bin position: %lu\n",
                                        file_position,
                                        file_position * 4);
    }

    return 0;
}
*/