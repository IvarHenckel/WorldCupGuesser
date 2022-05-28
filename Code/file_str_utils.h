#ifndef FILE_STR
#define FILE_STR

#include <stdio.h>

#define MAX_STRING_SIZE 128

/** Reads the string until EOF, \n or ;
 * returns the last char read. Not included in the string.
 * ';' -> there are more string on the same line
 * '\0' -> EOF
 * '\n' -> last string of the line*/
char read_string(FILE*, char*);

/**-1 => The string is not a positive integer*/ 
int string_to_pos_int(char*);

#endif