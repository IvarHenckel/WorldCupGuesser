#include <stdbool.h>
#include <stdio.h>
#include "list.h"

#ifndef PLAYER
#define PLAYER

typedef struct player_t player_t;

struct player_t {
    list_t* group_matches;
    list_t* playoff_matches;
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

bool match_equal(match_t*, match_t*);

bool match_reversed(match_t*, match_t*);

player_t* player_create(char*);

void player_destroy(player_t*);
#endif