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
    
    
    int p_id = 1;
    node_t* p_node = players->head;
    while (p_node != NULL) {    
        player_t* player = p_node->data;
        if (reference->group_matches->size != player->group_matches->size) {
            printf("ERROR: wrong number of matches for player %d\n", p_id);
            goto exit;
        }
        list_t* l = reference->group_matches;
        node_t* current = l->head;
        while (current != NULL) {
            match_t* m1 = current->data;

            list_t* l2 = player->group_matches;
            node_t* current2 = l2->head;
            while (current2 != NULL) {
                match_t* m2 = current2->data;
                
                if (match_equal(m1, m2)) {
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
                    break;
                }
            
                current2 = current2->next;
            }
            if (current2 == NULL) { // no equal match found
                printf("ERROR: no equal match found - %s vs %s", m1->team_a, m1->team_b);
                goto exit;
            }
            current = current->next;
        }

        printf("Final score for player %d was: %d\n", p_id, player->score);

        p_id++;
        p_node = p_node->next;
    }


    exit:
    player_destroy(reference);
    p_node = players->head;
    while (p_node != NULL) {
        player_destroy(p_node->data);
        node_t* tmp = p_node;   
        p_node = p_node->next;
        free(tmp);
    }
    free(players);
    return 0;
}


// It's easy to make an O(N^2) algorithm searching through the ref for the same match and check it.
// It would be better to go through the ref once and create a map matching match -> score
// Actually we know that the number of matches are a small number! So it's all O(1) regardless!