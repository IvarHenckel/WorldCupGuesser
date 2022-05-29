# With constans we can easily switch compiler for example
# And we don't have to type compiler flags over and over. 
CC=gcc
CFLAGS=-I

release: main.c list.c player.c file_str_utils.c
	$(CC) -o WorldCupGuesser.exe main.c list.c player.c file_str_utils.c -O3

# with main.c after hej: make knows that the hej command needs to run whenever main.c changes.
# main.o is also working, but why?
gdb: main.c list.c player.c file_str_utils.c
	$(CC) -o WorldCupGuesser.exe main.c list.c player.c file_str_utils.c -g
	gdb WorldCupGuesser.exe

# Run with make fsan
fsan:
	$(CC) -o WorldCupGuesser.exe main.c list.c player.c file_str_utils.c -fsanitize=address -g
	./WorldCupGuesser.exe ../TestData/ReferenceEuro2020.csv ../TestData/IvarEuro2020.csv