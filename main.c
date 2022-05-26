#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static int print_help();

static int print_help()
{

}

int main(int argc, char *argv[])
{
    printf("Welcome to WorldCupGuesser!\n");
    FILE *fp = fopen("TestData/PlayerIvar.csv", "r");
    if (!fp) {
        printf("Unable to open file!\n");
        exit(1);
    }
    printf("File open!\n");
    
    while (true) {
        int c = fgetc(fp);
        if (feof(fp)) break;
        printf("%c", c);
    }
    printf("\n");
    fclose(fp);
}