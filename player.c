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
    bool same_order = strcmp(m1->team_a->team_name, m2->team_a->team_name) == 0 && strcmp(m1->team_b->team_name, m2->team_b->team_name) == 0; 
    return same_order || match_reversed(m1, m2);
}

bool match_reversed(match_t* m1, match_t* m2)
{
    return strcmp(m1->team_a->team_name, m2->team_b->team_name) == 0 && strcmp(m1->team_b->team_name, m2->team_a->team_name) == 0;
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
    player->teams = list_create();
    while (!feof(fp)) {
        match_t* match = calloc(1, sizeof(match_t));
        //match->goal_scorers = list_create();
        char team_a_name[MAX_STRING_SIZE];
        char team_b_name[MAX_STRING_SIZE];
        bool ignore_line = false;
        char c = read_string(fp, team_a_name);
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
            char* goals_b = calloc(MAX_STRING_SIZE, sizeof(char)); //TODO should change to array just
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
            c = read_string(fp, team_b_name);
            ignore_line = (c != ';' && c != '\n' && c != '\0');
        }
        
        if (ignore_line) {
            //list_destroy(match->goal_scorers);
            free(match);
        } else {
            list_add(player->group_matches, match);
            team_t* team_a = NULL;
            team_t* team_b = NULL;
            for (int i = 0; i < player->teams->size; i++) {
                team_t* team = list_get(player->teams, i);
                if (strcmp(team->team_name, team_a_name) == 0) {
                    team_a = team;
                } else if(strcmp(team->team_name, team_b_name) == 0) {
                    team_b = team;
                }
            }
            if (team_a == NULL) {
                team_a = calloc(1, sizeof(team_t));
                team_a->group_matches = list_create();
                team_a->team_name = calloc(MAX_STRING_SIZE, sizeof(char));
                strcpy(team_a->team_name, team_a_name);
                list_add(player->teams, team_a);
            }
            if (team_b == NULL) {
                team_b = calloc(1, sizeof(team_t));
                team_b->group_matches = list_create();
                team_b->team_name = calloc(MAX_STRING_SIZE, sizeof(char));
                strcpy(team_b->team_name, team_b_name);
                list_add(player->teams, team_b);
            }
            match->team_a = team_a;
            match->team_b = team_b;
            list_add(team_a->group_matches, match);
            list_add(team_b->group_matches, match);
            printf("%s %d - %d %s\n",  match->team_a->team_name, match->goals_a, match->goals_b, match->team_b->team_name);
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
    for (int i = 0; i < player->group_matches->size; i++) {
        match_t* m = list_get(player->group_matches, i);
        free(m);
        //goal scorers
    }
    list_destroy(player->group_matches);
    for (int i = 0; i < player->teams->size; i++) {
        team_t* team = list_get(player->teams, i);
        free(team->team_name);
        list_destroy(team->group_matches);
        free(team);
    }
    list_destroy(player->teams);
    
    free(player);
}

/*-1 is the error code when this tool cannot determine group placement
(i.e everything is equal for two teams). This should be handled where this is called.
Just ask the user. But here we don't know the player number etc.*/
int team_group_placement(team_t* team)
{
    int opponents_lower_score = 0;
    for (int i = 0; i < team->group_matches->size; i++) {
        match_t* match = list_get(team->group_matches, i);
        team_t* opponent = match->team_b;
        if (match->team_b == team) {
            opponent = match->team_a;
        }
        if (opponent->group_score < team->group_score) opponents_lower_score++;
        else if (opponent->group_score == team->group_score) {
            if (opponent->goals_group - opponent->conceded_group < team->goals_group - team->conceded_group) {
                opponents_lower_score++;
            } else if (opponent->goals_group - opponent->conceded_group == team->goals_group - team->conceded_group) {
                if (opponent->goals_group < team->goals_group) {
                    opponents_lower_score++;
                } else if (opponent->goals_group == team->goals_group) {
                    // Note: this will print out 4 times currently, 2 for each input file (since they are currently the same and then twice since it goes for both teams)
                    printf("Group placement undefined! %s %d - %d %s. Needs to be handled by user.\n", team->team_name, team->group_score, opponent->group_score, opponent->team_name);
                    return -1;
                }
            }
        }
    }
    return opponents_lower_score;
}