#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "player.h"

int main(int argc, char *argv[])
{
    printf("Welcome to WorldCupGuesser!\n");
    player_t* reference = player_create("TestData/PlayerIvar.csv");
    player_t* p1 = player_create("TestData/PlayerIvar.csv");
    if (reference->group_matches->size != p1->group_matches->size) {
        printf("ERROR: wrong number of matches");
        goto exit;
    }

    list_t* l = reference->group_matches;
    node_t* current = l->head;
    while (current != NULL) {
        match_t* m1 = current->data;

        list_t* l2 = p1->group_matches;
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
                    p1->score += 2;
                } else if (goalDiff1 == 0 && goalDiff2 == 0) {
                    p1->score += 3;
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

    printf("Final score for player p1 was: %d\n", p1->score);

    exit:
    player_destroy(reference);
    player_destroy(p1);
    return 0;
}


// It's easy to make an O(N^2) algorithm searching through the ref for the same match and check it.
// It would be better to go through the ref once and create a map matching match -> score
// Actually we know that the number of matches are a small number! So it's all O(1) regardless!