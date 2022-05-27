# With constans we can easily switch compiler for example
# And we don't have to type compiler flags over and over. 
CC=gcc
CFLAGS=-I

# with main.c after hej: make knows that the hej command needs to run whenever main.c changes.
# main.o is also working, but why?
WorldCupGuesser: main.c list.c player.c
	$(CC) -o WorldCupGuesser.exe main.c list.c player.c


# Run with make fsan
fsan:
	$(CC) -o WorldCupGuesser.exe main.c list.c player.c -fsanitize=address -g
	./WorldCupGuesser.exe TestData/PlayerIvar.csv TestData/PlayerIvar.csv