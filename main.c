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
    player_t* reference = player_create(argv[1]);
    if (reference->teams->size != 24) {
        printf("Something is wrong\n");
    }
    
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
        if (player->teams->size != 24) {
            printf("Something is wrong\n");
        }
        for (int i = 0; i < reference->group_matches->size; i++) {
            match_t* m1 = list_get(reference->group_matches, i);

            bool found = false;
            for (int j = 0; j < player->group_matches->size && !found; j++) {
                match_t* m2 = list_get(player->group_matches, j);
                if (match_equal(m1, m2)) {
                    found = true;
                    int goals_a = m2->goals_a;
                    int goals_b = m2->goals_b;
                    if (match_reversed(m1, m2)) {
                        goals_a = m2->goals_b;
                        goals_b = m2->goals_a;
                    }
                    if (m1->goals_a == goals_a && m1->goals_b == goals_b) {
                        player->score += 2;
                    }
                    if ((goals_a > goals_b && m1->goals_a > m1->goals_b) || (goals_a < goals_b && m1->goals_a < m1->goals_b)) {
                        player->score += 2;
                    } else if (goals_a == goals_b && m1->goals_a == m1->goals_b) {
                        player->score += 3;
                    }

                    m2->team_a->goals_group += m2->goals_a;
                    m2->team_a->conceded_group += m2->goals_b;
                    m2->team_b->goals_group += m2->goals_b;
                    m2->team_b->conceded_group += m2->goals_a;
                    if (m2->goals_a < m2->goals_b) {
                        m2->team_b->group_score += 3;
                    } else if (m2->goals_a > m2->goals_b) {
                        m2->team_a->group_score += 3;
                    } else { // a draw
                        m2->team_a->group_score += 1;
                        m2->team_b->group_score += 1;
                    }
                    if (p == 0) { // only count reference group score once
                        m1->team_a->goals_group += m1->goals_a;
                        m1->team_a->conceded_group += m1->goals_b;
                        m1->team_b->goals_group += m1->goals_b;
                        m1->team_b->conceded_group += m1->goals_a;
                        if (m1->goals_a < m1->goals_b) {
                            m1->team_b->group_score += 3;
                        } else if (m1->goals_a > m1->goals_b) {
                            m1->team_a->group_score += 3;
                        } else { // a draw
                            m1->team_a->group_score += 1;
                            m1->team_b->group_score += 1;
                        }
                    }
                }
            }
            if (!found) { 
                printf("ERROR: no equal match found - %s vs %s", m1->team_a->team_name, m1->team_b->team_name);
                goto exit;
            }
        }

        for (int i = 0; i < reference->teams->size; i++) {
            team_t* team_reference = list_get(reference->teams, i);
            for (int j = 0; j < player->teams->size; j++) {
                team_t* team_player = list_get(player->teams, j);
                if (strcmp(team_reference->team_name, team_player->team_name) == 0) {    
                    if (team_reference->group_score == team_player->group_score) {
                        player->score += 2;
                    }
                    int team_player_place = team_group_placement(team_player);
                    if (team_player_place == -1) {
                        while (true) {
                            //TODO: need to write player name
                            printf("Group placement for Player %d and team %s needs to be written manually. Write placement 1-4:\n", p, team_player->team_name);
                            scanf("%d", &team_player_place);
                            if (team_player_place <= 4 || team_player_place >= 1) {
                                printf("Registered placement %d\n", team_player_place);
                                break;
                            } else {
                                printf("Answer incorrect format. Write placement 1-4:\n");    
                            }
                        }
                    }
                    int team_reference_place = team_group_placement(team_reference);
                    if (team_reference_place == -1) {
                        while (true) {
                            //TODO: need to write player name
                            printf("Group placement for reference and team %s needs to be written manually. Write placement 1-4:\n", team_reference->team_name);
                            scanf("%d", &team_reference_place);
                            if (team_reference_place <= 4 || team_reference_place >= 1) {
                                printf("Registered placement %d\n", team_reference_place);
                                break;
                            } else {
                                printf("Answer incorrect format. Write placement 1-4:\n");    
                            }
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
                    break;
                }
            }
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