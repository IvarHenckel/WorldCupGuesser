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
    player_destroy(reference);
    player_destroy(p1);
    return 0;
}