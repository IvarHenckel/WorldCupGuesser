#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "file_str_utils.h"

char read_string(FILE* fp, char* string)
{
    int i = 0;
    while (!feof(fp)) {
        char c = fgetc(fp);
        if (c == '\n' || c == ';') {
            string[i] = '\0';
            return c;
        } else if (i > MAX_STRING_SIZE) {
                printf("ERROR: MAX STRING SIZE EXCEEDED!\n");
                fclose(fp);
                exit(EXIT_FAILURE);
        } else {
            string[i] = c;
            i++;
        }
    }
    string[i] = '\0';
    return '\0';
}

int string_to_pos_int(char* string)
{
    int ret = 0;
    int i = 0;
    while (string[i] != '\0') {
        if (!isdigit(string[i])) return -1;
        ret = ret * 10 + (string[i] - '0');
        i++;
    }
    if (i == 0) return -1;
    return ret;
}