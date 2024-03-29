# WorldCupGuesser
A command line tool counting score for a football world cup betting game. Written in C.

Note: Since the number of teams in a world cup is small. I have not bothered to much to lower the time complexity.

## Rules of the game
There are four different categories, each category is calculated individually and then the score for each category is added together for the final score. I will add each rule here when this tool is able to calculate the score for the rule. There are more rules in the game that are currently not supported.

The group stage is now completely supported. I.e. all rules are calculated but more testing is necessary.

Compare to the original scores used I multiply everything with 2 to avoid decimals.
### Group Stage
- Correct winner in a match -> 2 p
- Correct guess on a draw for a match -> 3 p
- Correct results in a group match -> 2 p
- Correct placement in groups for a team -> 4 p 
- Correct number of group stage points for a team -> 2 p
- Correct number of goals scored AND goals conceded in groups -> 6 p
- Correct goal difference but not correct goals scored -> 4 p
- Correct goals scored OR (not both) goals conceded in groups -> 2 p

### Playoffs

### Goal Scorers

### Extra

## How to use
This tool takes as input a number of filenames. The first filename, reffered to as the reference file, corresponds to the file containing the actual results of the worldcup. The other files, refferred to as player files, are the results that each player guessed before the match was played, one file for each contender.  which To begin with *.csv files is the only file format supported. You can easily convert *.xlsx files to *.csv using Microsoft Excel.

### Format Reference File
This file contains the actual results for each matches on the following format:
    
    CountryA>;GoalsA;GoalsB;CountryB;PlayerName1;PlayerName2;

CountryA and CountryB are the two teams facing each other and GoalsA and GoalsB are simply the number of goals scored by the each team. After this follows a list of players, these are ordered such that PlayerName1 scored the first goal
and then PlayerName2 scores the second goal etc. There is no limit to how many players scored in the match, the end of this list of players is marked with a new line/row.

The matches need to be sorted in time, matches played earlier in the tournament needs to be placed above matches played later in the tournament. However, it is not necessary to sort matches in the group stage relative to each other, since the order of them is irrelevant. But the order of matches after the group stage is important to determine if match was played in the semifinals, the group stage or in the finals for example.

Note that we don't need to insert info about which team pplays in which group. The tool can just check which teams played each other four times. Similarly, to know the structure of the playoff tree we can just check which groups played each other in this file.

### Format Player Files
This file contains guesses for each matches on exactly the same format as for the reference solution. And again the matches need to be sorted in time.

For the player files no additional information is needed. This tool will calculate which games where played in which group and at which point in the tournament. As well as the total number of goals scored for a player etc.

The tool will ignore any lines that are not on this format. This means that a player can write down comments and summaries embedded in the file as long as they are on separate rows with a different format

### Flags
// Some flag should be used just to check that everything is on the correct format. 

// With one flag we can warn when a row is reached that has incorrect format.

// With one flag we can write out summaries for groups. Similar for players that scored etc.

// with one parameter we should be able to insert another csv file with legal player names. This can be used to check that there are no unknown player names in any players file.

## TODO:
1. Figure out how to handle tie breks in a more convenient way.... (It is different between different tournaments.)
2. Add tests. When doing so we also want to move parts to another file.
3. Make bool player_check_correct(player_t* reference); if reference is null we only check simple stuff (reference file).
4. Add better output. Write out points whenever points are scored rather than just
5. Add some type of delay. Either a delay of seconds for each point but we could also make it interactive so that the user presses something. Hmm actually, you could just redirect everything to a txt with ">" and handle the delay manually.

Note: I will leave this for now. Currently there are other things that are more worth my time but this can already be used to calculate score for the group stage. I am getting the wrong score on AnnaEuro2022.csv but she has made errors so it is hard to know how that was handled in the first place. I think we just miscalculated the first time, I get 18 when calculating again. This is just a hint that my tool can be very useful!

Note: Even if we do not use this tool for the next tournament (or if we just use it as a double check). It will add more test cases if we use the *.csv format. And if it calculates everything correctly we should be able to trust it the next tournament after that.