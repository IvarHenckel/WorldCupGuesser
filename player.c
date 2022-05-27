#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

#define MAX_STRING_SIZE 128

/** Reads the string until EOF, \n or ;
 * returns the last char read. Not included in the string.*/
static char read_string(FILE* fp, char* string)
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

// negative number are error codes
static int string_to_int(char* string)
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

bool match_equal(match_t* m1, match_t* m2)
{
    bool same_order = strcmp(m1->team_a, m2->team_a) == 0 && strcmp(m1->team_b, m2->team_b) == 0; 
    return same_order || match_reversed(m1, m2);
}

bool match_reversed(match_t* m1, match_t* m2)
{
    return strcmp(m1->team_a, m2->team_b) == 0 && strcmp(m1->team_b, m2->team_a) == 0;
}


player_t* player_create(char* filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("ERROR: Unable to open file - %s\n", filename);
        exit(EXIT_FAILURE);
    }
    player_t* player = calloc(1, sizeof(player_t));
    player->group_matches = list_create();
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
            list_add(player->group_matches, match);
            printf("%s %d - %d %s\n",  match->team_a, match->goals_a, match->goals_b, match->team_b);
        }

        while (c != '\n' && !feof(fp)) { // skip the rest of the line
            c = fgetc(fp);
        }
    }
    printf("\n");
    fclose(fp);
    return player;
}

void player_destroy(player_t* player)
{
    list_t* l = player->group_matches;
    node_t* current = l->head;
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
    free(player->group_matches);
    free(player);
}
