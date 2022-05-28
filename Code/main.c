#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "player.h"

int main(int argc, char *argv[])
{
    printf("Welcome to WorldCupGuesser!\n");
    if (argc < 2) {
        printf("Error: No reference file provided\n");
    }
    printf("Reference file used: %s\n", argv[1]);
    player_t* reference = player_create(argv[1], 0);
    
    if (argc < 3) {
        printf("Error: At least one player file must be provided\n");
    }
    list_t* players = list_create();
    for (int i = argc - 1; i >= 2; i--) {
        printf("Player %d uses file: %s\n", i - 1, argv[i]);
        list_add(players, player_create(argv[i], i - 1));
    }
    
    for (int p = 0; p < players->size; p++) { 
        player_t* player = list_get(players, p);
        if (reference->group_matches->size != player->group_matches->size) {
            printf("ERROR: wrong number of matches for player %d\n", player->id);
            goto exit;
        }
        for (int i = 0; i < reference->group_matches->size; i++) {
            match_t* match_ref = list_get(reference->group_matches, i);
            bool found = false;
            for (int j = 0; j < player->group_matches->size && !found; j++) {
                match_t* match_player = list_get(player->group_matches, j);
                if (match_equal(match_ref, match_player)) {
                    found = true;
                    int goals_a = match_player->goals_a;
                    int goals_b = match_player->goals_b;
                    if (match_reversed(match_ref, match_player)) {
                        goals_a = match_player->goals_b;
                        goals_b = match_player->goals_a;
                    }
                    if (match_ref->goals_a == goals_a && match_ref->goals_b == goals_b) {
                        player->score += 2;
                    }
                    if ((goals_a > goals_b && match_ref->goals_a > match_ref->goals_b) || (goals_a < goals_b && match_ref->goals_a < match_ref->goals_b)) {
                        player->score += 2;
                    } else if (goals_a == goals_b && match_ref->goals_a == match_ref->goals_b) {
                        player->score += 3;
                    }
                }
            }
            if (!found) { 
                printf("ERROR: no equal match found - %s vs %s", match_ref->team_a->team_name, match_ref->team_b->team_name);
                goto exit;
            }
        }

        for (int i = 0; i < reference->teams->size; i++) {
            team_t* team_reference = list_get(reference->teams, i);
            team_t* team_player = player_get_team(player, team_reference->team_name);
            
            if (team_reference->group_score == team_player->group_score) {
                player->score += 2;
            }
            int team_player_place = team_group_placement(team_player);
            while (team_player_place > 4 || team_player_place < 1) {
                printf("Group placement for Player %d and team %s needs to be written manually. Write placement 1-4:\n", player->id, team_player->team_name);
                scanf("%d", &team_player_place);
                if (team_player_place <= 4 && team_player_place >= 1) {
                    printf("Registered placement %d\n", team_player_place);
                    break;
                } else {
                    printf("Answer incorrect format. Write placement 1-4:\n");    
                }
            }
        
            int team_reference_place = team_group_placement(team_reference);
            while (team_reference_place > 4 || team_reference_place < 1) {
                printf("Group placement for reference and team %s needs to be written manually. Write placement 1-4:\n", team_reference->team_name);
                scanf("%d", &team_reference_place);
                if (team_reference_place <= 4 && team_reference_place >= 1) {
                    printf("Registered placement %d\n", team_reference_place);
                    break;
                } else {
                    printf("Answer incorrect format. Write placement 1-4:\n");    
                }
            }
            if (team_player_place == team_reference_place) {
                player->score += 4;
            }
            if (team_reference->goals_group == team_player->goals_group && team_reference->conceded_group == team_player->conceded_group) {
                player->score += 6;
            } else if (team_reference->goals_group - team_reference->conceded_group ==  team_player->goals_group - team_player->conceded_group) {
                player->score += 4;
            } else if (team_reference->goals_group == team_player->goals_group || team_reference->conceded_group == team_player->conceded_group) {
                player->score += 2;
            }
        }
        printf("Final score for player %d was: %d\n", player->id, player->score);
    }

    exit:
    player_destroy(reference);
    for (int i = 0; i < players->size; i++) {
        player_destroy(list_get(players, i));
    }
    list_destroy(players);
    return 0;
}