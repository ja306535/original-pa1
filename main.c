/************************************
Name:Jalil Fountain
Date:1/21/24
This assignment will set up a soccer tournament and print information based on user input
 ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
    char *name; //Player name (dynamically allocated)
    int totalGoals; //Total goals scored by the player(needs to be counted based on the match)
}Player;
typedef struct Team {
    char *name; // Name of the team (dynamically allocated)
    Player **players; //An array of Player pointers where each pointer is pointing to a player
    int playerCount; // Number of players in the team
}Team;
typedef struct Goal {
    Player *scorer; // Pointer to the player who scored
    int time; // Time of the goal in minutes
}Goal;
typedef struct Match {
    Team *team1; // Pointer to Team 1
    Team *team2; // Pointer to Team 2
    Goal *goals; // Array of goals scored in the match
    int goalCount; // Total number of goals in the match
} Match;

typedef struct MatchWinnerTeam
{
    Team *winnerTeam; //pointer to the winner team if any.
    int isDraw; //set to 1 if it was a draw. In that case winnerTeam will be NULL
}MatchWinnerTeam;
/*******************
 * this function dynamically allocates memory for a player name,
 * stores the name into a dynamically allocated string,
 * initializes total goals for that player to 0,
 * and returns a player pointer that will be used in the player struct
 ********************/
Player* createPlayer(char *name) {

 Player *p =malloc(sizeof(Player)); // create a pointer that points to a box that can hold a player struct
 p->totalGoals=0; // set total goals for the player to 0
 int len=strlen(name)+1; // find out how much space you need based on the players name
 p->name=malloc(len*sizeof(char)); // allocate the space for name
 strcpy(p->name,name); // store the name


 return p; // return the pointer
}
/*******************
 *This function recieves the team name and the number of players on the team
 *dynamically allocates memory for the team name and dynamically allocates memory for an array of size <playercount>
 ********************/
Team* createTeam(char *name, int playerCount) {

Team *t =malloc(sizeof(Team)); // create a pointer that points to a box that can hold a team struct
t->playerCount=playerCount; // set player count to the number the user enters
int len=strlen(name)+1;// find out how much space you need based on the team name
t->name=malloc(len*sizeof(char)); // allocate the space for team name
strcpy(t->name,name); // store that name

t->players=malloc(playerCount*sizeof(Player*)); // allocate memory for an array of player pointers that point to a player block

return t;

 }


/*******************
 *This function returns the player pointer in the playerList with the given name passed to the function
 *if the player is not found it should return Null
 *for this function maybe iterate through the list and use strcmp if it returns a non zero return Null
 ********************/
Player* getPlayerByName(char* name, Player** playerList, int numPlayers) {
    
    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(name, playerList[i]->name) == 0) {
            return playerList[i]; //return the pointer if player found
        }
    }
    return NULL; // return NULL if not found
}
/*******************
 *This function Returns the Team pointer in the teamList with the given team name passed to this function
 *if the team is not found it should return NUll
 *for this function maybe iterate through the list and use strcmp if it returns a non zero return Null
 ********************/
Team* getTeamByTeamName(Team** teamList, int numTeams, char *tname) {
    
    for (int i = 0; i < numTeams; i++) {
        if (strcmp(tname, teamList[i]->name) == 0) {
            return teamList[i]; // return the pointer if team found
        }
    }
    return NULL; // return NULL if not found
}

/*******************
 *This function gets a pointer to a match, pointer to a player, and time,
 * adds it to the goal list for the match its pointing to and updates total goals for the player and total goals scored in the match.
 * (use realloc to increase goal array size)
 ********************/
void recordGoal(Match *match, Player *player, int time) {
    // increase the size of the goals array
    match->goals = realloc(match->goals, (match->goalCount + 1) * sizeof(Goal));
    
    // add the new goal directly to the array
    match->goals[match->goalCount].scorer = player;
    match->goals[match->goalCount].time = time;
    match->goalCount++;

    // update the player's total goals
    player->totalGoals++;
}
/*******************
 *This function gets a reference to a match and prints the match details including the name of any player who scored a hat trick and returns the integer 1 
 *if applicable and 0 if no hat trick was found
 ********************/
int printHatTrickPlayers(Match *match) {  
    // allocate arrays for player pointers and goal counts
    Player **pp = calloc(match->goalCount, sizeof(Player *));
    int *gc = calloc(match->goalCount, sizeof(int));
    int tracker = 0;
    int hatTrickFound = 0;

    // loop through each goal in the match
    for (int i = 0; i < match->goalCount; i++) {
        Player *scorer = match->goals[i].scorer;
        int found = 0;

        // search for the player in the list
        for (int j = 0; j < tracker; j++) {
            if (pp[j] == scorer) {
                gc[j]++;  // update goal count for the scorer
                found = 1;
            }
        }

        // if the player is not found, add them to the list
        if (found==0) {
            pp[tracker] = scorer;
            gc[tracker] = 1; // update the new players goal to 1 
            tracker++;
        }
    }

    // check for hat tricks (3 or more goals)
    for (int i = 0; i < tracker; i++) {
        if (gc[i] >= 3) {
            hatTrickFound = 1;
            printf("Hat trick found in %s vs %s: %s %d\n",
                   match->team1->name, match->team2->name, pp[i]->name, gc[i]);
        }
    }
    
if (hatTrickFound==0) {
    printf("No hat trick scored in the tournament.\n");
}
    // free arrays once done withthem
    free(pp);
    free(gc);
    return hatTrickFound;
}
/*******************
 *This function gets recieves the list of matches,match count, and two team names
 * and dynamically allocates MatchWinnerTeam struct that stores the winner and returns the result or NULL if no match is found
 ********************/
MatchWinnerTeam* getWinningTeam(Match *matches, int matchCount, char *team1Name, char *team2Name) {
    
    for (int i = 0; i < matchCount; i++) {
        // check if the match involves the two teams in any order if not return NULL
        if ((strcmp(matches[i].team1->name, team1Name) == 0 && strcmp(matches[i].team2->name, team2Name) == 0) || (strcmp(matches[i].team1->name, team2Name) == 0 && strcmp(matches[i].team2->name, team1Name) == 0)) {

            int team1Goals = 0, team2Goals = 0;

            // total goals for team1 players
            for (int j = 0; j < matches[i].team1->playerCount; j++) {
                team1Goals += matches[i].team1->players[j]->totalGoals;
            }

            // total goals for team2 players
            for (int k = 0; k < matches[i].team2->playerCount; k++) {
                team2Goals += matches[i].team2->players[k]->totalGoals;
            }

            // DMA a result struct to store status depending on winner or draw
            MatchWinnerTeam *result = malloc(sizeof(MatchWinnerTeam));
            if (team1Goals > team2Goals) {
                result->winnerTeam = matches[i].team1;
                result->isDraw = 0;
            } else if (team2Goals > team1Goals) {
                result->winnerTeam = matches[i].team2;
                result->isDraw = 0;
            } else {
                result->winnerTeam = NULL;
                result->isDraw = 1;
            }

            // return the result
            return result;
        }
    }
    // if no match is found return NULL
    return NULL;
}
/*******************
 *This function returns tha champion of the tournament based on a specific point system
 *for this function use strcmp to compare to getWinningTeam function 
 ********************/
Team* getChampion(Team **teamList, int teamCount, Match *matchList, int matchCount) {
    // array to track points for each team
    int *points =calloc(teamCount, sizeof(int));
    
    // process each match
    for (int i = 0; i < matchCount; i++) {
        MatchWinnerTeam *result = getWinningTeam(matchList, matchCount, matchList[i].team1->name, matchList[i].team2->name);
        
            if (result->isDraw) {
                // add 1 point to both teams
                for (int j = 0; j < teamCount; j++) {
                    if (strcmp(teamList[j]->name, matchList[i].team1->name) == 0) {
                        points[j] += 1;
                    }
                    if (strcmp(teamList[j]->name, matchList[i].team2->name) == 0) {
                        points[j] += 1;
                    }
                }
            } else {
                //  give 3 points for the winner
                for (int j = 0; j < teamCount; j++) {
                    if (strcmp(teamList[j]->name, result->winnerTeam->name) == 0) {
                        points[j] += 3;  // winner gets 3 points
                    }
                }
            }
        
    }

// find the team with the highest points
//set max points and champ index to -1 because 0 is valid
    int maxPoints = -1;
    int championIndex = -1;
    for (int i = 0; i < teamCount; i++) {
        if (points[i] > maxPoints) {
            maxPoints = points[i];
            championIndex = i;
        }
    }
    // return the champion 
    return teamList[championIndex];
}

int main(void) {
    int numTeams; // number of teams
    int numMatches; // number of matches
    int numCommands; // number of commands
    int numGoals; // number of goals
    int playerCount; // number of players
    int goalTime; // goal time
    int totalPlayers = 0; // total players
    char command[21]; // command string
    char teamName[21]; // team name used for each team
    char playerName[21]; // player name
    char team1Name[21], team2Name[21]; // team names used for matches

    printf("Enter the number of teams, matches, and commands: ");
    scanf("%d %d %d", &numTeams, &numMatches, &numCommands);

    if (numTeams == 0) {
        printf("You must have at least 1 team to continue\n");
        return 0;
    }
    // allocate memory for player list
    Player **playerList = NULL;

    // allocate memory for the team list
    Team **teamList = malloc(numTeams * sizeof(Team*));

    // input team and player info
    for (int i = 0; i < numTeams; i++) {
        if (numTeams == 1) {
            printf("You can add players to the team, but you will not be able to start a match/make commands\n");
            printf("Enter team 1 name and player count: ");
            scanf("%s %d", teamName, &playerCount);

            // Allocate memory for players
            Player **playerList = malloc(playerCount * sizeof(Player*));

            for (int i = 0; i < playerCount; i++) {
                printf("Enter player %d name: ", i + 1);
                scanf("%s", playerName);

                playerList[i] = createPlayer(playerName);
            }
            printf("Team name: %s, players:\n", teamName);
            for (int i = 0; i < playerCount; i++) {
                printf("Player %d: %s\n", i + 1, playerList[i]->name);
            }
            for (int i = 0; i < playerCount; i++) {
                free(playerList[i]->name);
            }
            free(playerList);
            return 0;
        }

        // if numTeams is more than 1 proceed
        printf("Enter team %d name and player count: ", i + 1);
        scanf("%s %d", teamName, &playerCount);

        // allocate memory for the team
        teamList[i] = createTeam(teamName, playerCount);

        // input player info for each team
        for (int j = 0; j < playerCount; j++) {
            printf("Enter player %d name: ", j + 1);
            scanf("%s", playerName);
            teamList[i]->players[j] = createPlayer(playerName);

            // realloc playerList to hold additional player
            playerList = realloc(playerList, (totalPlayers + 1) * sizeof(Player*));
            // add the player to playerList
            playerList[totalPlayers] = teamList[i]->players[j];
            totalPlayers++;
        }
    }

    // allocate memory for matches of size Match
    Match *matchList = malloc(numMatches * sizeof(Match));

    for (int i = 0; i < numMatches; i++) {
        printf("Enter Team 1 and Team 2 for match %d: ", i + 1);
        scanf("%s %s", team1Name, team2Name);

        Team *team1 = getTeamByTeamName(teamList, numTeams, team1Name);
        Team *team2 = getTeamByTeamName(teamList, numTeams, team2Name);

         // check if teams are found
        if (team1 == NULL || team2 == NULL) {
            printf("One or both teams not found.\n");
            return 0;
        }
        //store teams in matchList to handle matches
        matchList[i].team1 = team1;
        matchList[i].team2 = team2;

        // input goals
        printf("Enter the number of goals for match %d: ", i + 1);
        scanf("%d", &numGoals);

        // allocate memory for goals
        matchList[i].goalCount = numGoals;
        matchList[i].goals = malloc(numGoals * sizeof(Goal));

        // enter scorer and goal time
        for (int g = 0; g < numGoals; g++) {
            printf("Enter scorer %d name and goal time: ", g + 1);
            scanf("%s %d", playerName, &goalTime);

            // find player who scored the goal in playerList
            Player *scorer = NULL;
            for (int j = 0; j < totalPlayers && !scorer; j++) {
                if (strcmp(playerList[j]->name, playerName) == 0) {
                    scorer = playerList[j];
                }
            }

            if (scorer != NULL) {
                recordGoal(&matchList[i], scorer, goalTime);
            }
        }
    }

    // process commands
    printf("Enter the commands:\n");
    for (int i = 0; i < numCommands; i++) {
        scanf("%s", command);

        // if user enters hattrick, call function
        if (strcmp(command, "hattrick") == 0) {
            for (int j = 0; j < numMatches; j++) {
                printHatTrickPlayers(&matchList[j]);
            }
        }

        // if user calls max_goal_player
        else if (strcmp(command, "max_goal_player") == 0) {
            Player *maxGoalPlayer = playerList[0]; // initialize with the first player from the playerList
            for (int i = 0; i < totalPlayers; i++) {
                Player *p = playerList[i]; //start at the first player
                if (p->totalGoals > maxGoalPlayer->totalGoals) {
                    maxGoalPlayer = p;
                }
            }
            // output the result
            printf("Player with the most goals: %s (%d goals)\n", maxGoalPlayer->name, maxGoalPlayer->totalGoals);
        }

        // if user enters winner, scan the team names
        else if (strcmp(command, "winner") == 0) {
            scanf("%s %s", team1Name, team2Name);
            if (numMatches == 0) {
                printf("There is no winner because there was no match\n");
            } else {
                MatchWinnerTeam *winningTeam = getWinningTeam(matchList, numMatches, team1Name, team2Name);
                if (winningTeam != NULL) {
                    if (winningTeam->isDraw) {
                        printf("%s vs %s match drawn.\n", team1Name, team2Name);
                    } else {
                        printf("Winner of %s vs %s is %s.\n", team1Name, team2Name, winningTeam->winnerTeam->name);
                    }
                } else {
                    printf("No match between %s and %s.\n", team1Name, team2Name);
                }
            }
        }

        else if (strcmp(command, "champion") == 0) {
            if (numMatches == 0) {
                printf("There is no champion because there was no match\n");
            } else {
                Team *champion = getChampion(teamList, numTeams, matchList, numMatches);
                printf("The champion is: %s\n", champion->name);
            }
        }
    }

    // free memory
    for (int i = 0; i < numTeams; i++) {
        for (int j = 0; j < teamList[i]->playerCount; j++) {
            free(teamList[i]->players[j]); // free each player
        }
        free(teamList[i]->players); // free player array within team
        free(teamList[i]); // free the team itself
    }
    free(teamList); // free the team list

    for (int i = 0; i < numMatches; i++) {
        free(matchList[i].goals); // free goal array for each match
    }
    free(matchList); // free match list
    free(playerList); // free player list

    return 0;
}