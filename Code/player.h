#include <stdbool.h>
#include <stdio.h>
#include "list.h"

#ifndef PLAYER
#define PLAYER


typedef struct team_t team_t;

struct team_t {
    list_t* group_matches;
    char* team_name;
    int group_score;
    int goals_group;
    int conceded_group;
};

typedef struct player_t player_t;

struct player_t {
    list_t* group_matches;
    list_t* teams;
    list_t* playoff_matches;
    int score;
    int id;
};

typedef struct match_t match_t;

struct match_t {
    //list_t* goal_scorers;//list of player names
    team_t* team_a;
    team_t* team_b;
    int goals_a;
    int goals_b;
};

bool match_equal(match_t*, match_t*);

bool match_reversed(match_t*, match_t*);

player_t* player_create(char*, int);

void player_destroy(player_t*);

team_t* player_get_team(player_t*, char*);

/*-1 is the error code when this tool cannot determine group placement
(i.e everything is equal for two teams). This should be handled where this is called.
Just ask the user. Inside this function we don't know the player number etc.*/
int team_group_placement(team_t*);
#endif