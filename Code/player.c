#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_str_utils.h"
#include "player.h"

static team_t* team_create(char*);

bool match_equal(match_t* m1, match_t* m2)
{
    bool same_order = strcmp(m1->team_a->team_name, m2->team_a->team_name) == 0 && strcmp(m1->team_b->team_name, m2->team_b->team_name) == 0; 
    return same_order || match_reversed(m1, m2);
}

bool match_reversed(match_t* m1, match_t* m2)
{
    return strcmp(m1->team_a->team_name, m2->team_b->team_name) == 0 && strcmp(m1->team_b->team_name, m2->team_a->team_name) == 0;
}

player_t* player_create(char* filename, int id)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("ERROR: Unable to open file - %s\n", filename);
        exit(EXIT_FAILURE);
    }
    player_t* player = calloc(1, sizeof(player_t));
    player->group_matches = list_create();
    player->teams = list_create();
    player->id = id;
    while (!feof(fp)) {
        char team_a_name[MAX_STRING_SIZE];
        char team_b_name[MAX_STRING_SIZE];
        int goals_a;
        int goals_b;
        bool ignore_line = false;
        char c = read_string(fp, team_a_name);
        ignore_line = c != ';'; 
        if (!ignore_line) {
            char goals_a_str[MAX_STRING_SIZE];
            c = read_string(fp, goals_a_str);
            ignore_line = c != ';';
            if (!ignore_line) {
                goals_a = string_to_pos_int(goals_a_str);
                ignore_line = goals_a < 0;
            }
        }
        if (!ignore_line) {
            char goals_b_str[MAX_STRING_SIZE];
            c = read_string(fp, goals_b_str);
            ignore_line = c != ';';
            if (!ignore_line) {
                goals_b = string_to_pos_int(goals_b_str);
                ignore_line = goals_b < 0;
            }
        }
        if (!ignore_line) {
            c = read_string(fp, team_b_name);
            ignore_line = (c != ';' && c != '\n' && c != '\0');
        }
        
        if (!ignore_line) {
            team_t* team_a = player_get_team(player, team_a_name);
            if (team_a == NULL) {
                team_a = team_create(team_a_name);
                list_add(player->teams, team_a);
            }
            team_t* team_b = player_get_team(player, team_b_name);
            if (team_b == NULL) {
                team_b = team_create(team_b_name);
                list_add(player->teams, team_b);
            }
            match_t* match = calloc(1, sizeof(match_t));
            match->goals_a = goals_a;
            match->goals_b = goals_b;
            match->team_a = team_a;
            match->team_b = team_b;
            match->team_a->goals_group += match->goals_a;
            match->team_a->conceded_group += match->goals_b;
            match->team_b->goals_group += match->goals_b;
            match->team_b->conceded_group += match->goals_a;
            if (match->goals_a < match->goals_b) {
                match->team_b->group_score += 3;
            } else if (match->goals_a > match->goals_b) {
                match->team_a->group_score += 3;
            } else { // draw
                match->team_a->group_score += 1;
                match->team_b->group_score += 1;
            }
            list_add(player->group_matches, match);
            list_add(team_a->group_matches, match);
            list_add(team_b->group_matches, match);
        }

        while (c != '\n' && !feof(fp)) { // skip the rest of the line
            c = fgetc(fp);
        }
    }
    fclose(fp);
    return player;
}

void player_destroy(player_t* player)
{
    for (int i = 0; i < player->group_matches->size; i++) {
        match_t* m = list_get(player->group_matches, i);
        free(m);
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

team_t* player_get_team(player_t* player, char* team_name)
{
    for (int j = 0; j < player->teams->size; j++) {
        team_t* team = list_get(player->teams, j);
        if (strcmp(team->team_name, team_name) == 0) {
            return team;
        }
    }
    return NULL;
}

static team_t* team_create(char* team_name)
{
    team_t* team = calloc(1, sizeof(team_t));
    team->group_matches = list_create();
    team->team_name = calloc(MAX_STRING_SIZE, sizeof(char));
    strcpy(team->team_name, team_name);
    return team;
}

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
                // Some tournaments use number of goals as tiebreaker others use the match results when the two teams faced each other etc.
                // Note: this will happen 4 times currently, 2 for each input file (since they are currently the same and then twice since it goes for both teams)
                printf("Group placement undefined! %s %d - %d %s. Needs to be handled by user.\n", team->team_name, team->group_score, opponent->group_score, opponent->team_name);
                return -1;
            }
        }
    }
    return 4 - opponents_lower_score;
}