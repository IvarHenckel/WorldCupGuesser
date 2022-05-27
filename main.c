#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "player.h"

int main(int argc, char *argv[])
{
    printf("Welcome to WorldCupGuesser!\n");
    if (argc < 2) {
        printf("Error: No reference file provided\n");
    }
    printf("Reference file used: %s\n", argv[1]);
    player_t* reference = player_create(argv[1]);
    
    if (argc < 3) {
        printf("Error: At least one player file must be provided\n");
    }
    list_t* players = list_create();
    for (int i = argc - 1; i >= 2; i--) {
        printf("Player %d uses file: %s\n", i - 1, argv[i]);
        list_add(players, player_create(argv[i]));
    }
    
    for (int p = 0; p < players->size; p++) { 
        player_t* player = list_get(players, p);
        if (reference->group_matches->size != player->group_matches->size) {
            printf("ERROR: wrong number of matches for player %d\n", p + 1);
            goto exit;
        }
        for (int i = 0; i < reference->group_matches->size; i++) {
            match_t* m1 = list_get(reference->group_matches, i);

            bool found = false;
            for (int j = 0; j < player->group_matches->size && !found; j++) {
                match_t* m2 = list_get(player->group_matches, j);
                if (match_equal(m1, m2)) {
                    found = true;
                    int goalDiff1 = m1->goals_a - m1->goals_b;
                    int goalDiff2 = m2->goals_a - m2->goals_b;
                    if (match_reversed(m1, m2)) {
                        goalDiff2 = m2->goals_b - m2->goals_a;
                    }
                    if ((goalDiff1 > 0 && goalDiff2 > 0) || (goalDiff1 < 0 && goalDiff2 < 0)) {
                        player->score += 2;
                    } else if (goalDiff1 == 0 && goalDiff2 == 0) {
                        player->score += 3;
                    }
                }
            }
            if (!found) { // no equal match found
                printf("ERROR: no equal match found - %s vs %s", m1->team_a->team_name, m1->team_b->team_name);
                goto exit;
            }
        }

        for (int i = 0; i < reference->group_matches->size; i++) {
            
        }

        printf("Final score for player %d was: %d\n", p + 1, player->score);
    }


    exit:
    player_destroy(reference);
    for (int i = 0; i < players->size; i++) {
        player_destroy(list_get(players, i));
    }
    list_destroy(players);
    return 0;
}


// It's easy to make an O(N^2) algorithm searching through the ref for the same match and check it.
// It would be better to go through the ref once and create a map matching match -> score
// Actually we know that the number of matches are a small number! So it's all O(1) regardless!