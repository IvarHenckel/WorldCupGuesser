#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

#define MAX_STRING_SIZE 128

typedef struct player_t player_t;

struct player_t {
    list_t* group_matches;
    list_t* playoff_matches;
    FILE* file;
    int score;
};

typedef struct match_t match_t;

struct match_t {
    list_t* goal_scorers;//list of player names
    char* team_a;
    char* team_b;
    int goals_a;
    int goals_b;
};

typedef struct match_t match_t;

char read_string(FILE*, char*);

int string_to_int(char*);

/** Reads the string until EOF, \n or ;
 * returns the last char read. Not included in the string.
*/
char read_string(FILE* fp, char* string)
{
    int i = 0;
    while (!feof(fp)) {
        int c = fgetc(fp);
        if (c == '\n' || c == ';') {
            string[i] = '\0';
            return c;
        } else if (i > MAX_STRING_SIZE) {
                printf("ERROR: MAX STRING SIZE EXCEEDED!");
                exit(1);
        } else {
            string[i] = c;
            i++;
        }
    }
    string[i] = '\0';
    return '\0';
}

// negative number are error codes
int string_to_int(char* string)
{
    int ret = 0;
    int i = 0;
    while (string[i] != '\0') {
        if (!isdigit(string[i])) {
            return -1;
        }
        ret = ret * 10 + (string[i] - '0');
        i++;
    }
    if (i == 0) {
        return -1;
    }
    return ret;
}


int main(int argc, char *argv[])
{
    printf("Welcome to WorldCupGuesser!\n");
    FILE *fp = fopen("TestData/PlayerIvar.csv", "r");
    if (!fp) {
        printf("Unable to open file!\n");
        return 1;
    }
    printf("File open!\n");

    player_t* reference = calloc(1, sizeof(player_t));
    reference->group_matches = list_create();
    player_t* p1 = calloc(1, sizeof(player_t));
    p1->group_matches = list_create();
    reference->file = fp;
    p1->file = fp;
    while (!feof(fp)) {
        match_t* match = calloc(1, sizeof(match_t));
        //match->goal_scorers = list_create();
        match->team_a = calloc(MAX_STRING_SIZE, sizeof(char));
        match->team_b = calloc(MAX_STRING_SIZE, sizeof(char));
        bool ignore_line = false;
        char c = read_string(fp, match->team_a);
        ignore_line = c != ';'; 
        if (!ignore_line) {
            char* goals_a = calloc(MAX_STRING_SIZE, sizeof(char));
            c = read_string(fp, goals_a);
            ignore_line = c != ';';
            if (!ignore_line) {
                //convert to number
                match->goals_a = string_to_int(goals_a);
                ignore_line = match->goals_a < 0;
            }
            free(goals_a);
        }
        if (!ignore_line) {
            char* goals_b = calloc(MAX_STRING_SIZE, sizeof(char));
            c = read_string(fp, goals_b);
            ignore_line = c != ';';
            if (!ignore_line) {
                //convert to number
                match->goals_b = string_to_int(goals_b);
                ignore_line = match->goals_b < 0;
            }
            free(goals_b);
        }
        if (!ignore_line) {
            c = read_string(fp, match->team_b);
            ignore_line = (c != ';' && c != '\n' && c != '\0');
        }
        
        if (ignore_line) {
            //list_destroy(match->goal_scorers);
            free(match->team_a);
            free(match->team_b);
            free(match);
        } else {
            list_add(reference->group_matches, match);
            printf("%s %d - %d %s\n",  match->team_a, match->goals_a, match->goals_b, match->team_b);
        }

        while (c != '\n' && !feof(fp)) { // skip the rest of the line
            c = fgetc(fp);
        }
    }
    printf("\n");
    fclose(fp);

    node_t* current = reference->group_matches->head;
    while (current != NULL) {
        node_t* tmp = current;
        current = current->next;
        match_t* m = tmp->data;
        free(m->team_a);
        free(m->team_b);
        free(m);
        //goal scorers
        free(tmp);
    }
    free(reference->group_matches);
    current = p1->group_matches->head;
    while (current != NULL) {
        node_t* tmp = current;
        current = current->next;
        match_t* m = tmp->data;
        free(m->team_a);
        free(m->team_b);
        free(m);
        //goal scorers
        free(tmp);
    }
    free(p1->group_matches);

    free(reference);
    free(p1);
    return 0;
}