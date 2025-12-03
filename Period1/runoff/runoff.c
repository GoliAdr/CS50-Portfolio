#include <cs50.h>      // get_int(), get_string(), string
#include <stdio.h>     // printf()
#include <string.h>    // strcmp() to check if two names are exactly the same
#include <limits.h>    // INT_MAX (used as a very large number for find_min)




#define MAX_VOTERS 100       // maximum number of voters allowed
#define MAX_CANDIDATES 9     // maximum number of candidates allowed



int preferences[MAX_VOTERS][MAX_CANDIDATES]; // for each voter: store which candidate they put in rank 1, rank 2, etc.



typedef struct
{
    string name;        // candidate’s name
    int votes;          // how many votes they currently have
    bool eliminated;    // true if removed from the running
}

candidate;


candidate candidates[MAX_CANDIDATES]; // list of all candidates
int candidate_count;                  // how many candidates we have
int voter_count;                      // how many voters we have total
if (argc < 2)
{
    printf("Usage: ./runoff [candidate ...]\n");
    return 1;           // need at least 1 candidate
}



candidate_count = argc - 1;          // argv[1..] are names
if (candidate_count > MAX_CANDIDATES)
{
    printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
    return 2;
}



// copy all candidate names from argv into our list
for (int i = 0; i < candidate_count; i++)
{
    candidates[i].name = argv[i + 1];  // set the name
    candidates[i].votes = 0;           // no votes yet
    candidates[i].eliminated = false;  // nobody eliminated at the start
}



voter_count = get_int("Number of voters: ");



if (voter_count > MAX_VOTERS)
{
    printf("Maximum number of voters is %i\n", MAX_VOTERS);
    return 3;
}
for (int i = 0; i < voter_count; i++)       // for every voter
{
    for (int j = 0; j < candidate_count; j++)  // for every rank
    {
        string name = get_string("Rank %i: ", j + 1);

        // try to save this rank for voter i
        if (!vote(i, j, name))     // vote() returns false if name not found
        {
            printf("Invalid vote.\n");
            return 4;
        }
    }
    printf("\n");
}



while (true)
{
    tabulate();   // count votes for each non-eliminated candidate

    if (print_winner())   // check if someone has > 50%
    {
        break;            // winner found → done
    }

    int min = find_min(); // smallest vote count among remaining candidates
    if (is_tie(min))      // are all remaining tied?
    {
        // print all non-eliminated candidates
        for (int i = 0; i < candidate_count; i++)
        {
            if (!candidates[i].eliminated)
            {
                printf("%s\n", candidates[i].name);
            }
        }
        break;
    }

    eliminate(min);       // eliminate all candidates with the minimum votes

    // reset votes to 0 so the next round can recalculate them
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].votes = 0;
    }
}



bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // did the user type a name that matches a candidate?
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;   // save candidate index
            return true;                    // success
        }
    }
    return false;   // no such candidate → invalid vote
}


for (int v = 0; v < voter_count; v++)      // for each voter
{
    for (int r = 0; r < candidate_count; r++)   // go through their ranked list
    {
        int idx = preferences[v][r];       // candidate index for this rank

        // if this candidate is still in the race, give them 1 vote
        if (!candidates[idx].eliminated)
        {
            candidates[idx].votes++;
            break;   // stop after first active candidate
        }
    }
}


int majority = voter_count / 2;  // more than half

for (int i = 0; i < candidate_count; i++)
{
    // check only active candidates
    if (!candidates[i].eliminated && candidates[i].votes > majority)
    {
        printf("%s\n", candidates[i].name);   // winner
        return true;
    }
}
return false;   // no winner yet



int min = INT_MAX;   // start with a very large number

for (int i = 0; i < candidate_count; i++)
{
    if (!candidates[i].eliminated && candidates[i].votes < min)
    {
        min = candidates[i].votes;    // update lowest vote count
    }
}
return min;


for (int i = 0; i < candidate_count; i++)
{
    if (!candidates[i].eliminated && candidates[i].votes != min)
    {
        return false;   // found someone with more → not a tie
    }
}
return true;            // all remaining have the same votes


for (int i = 0; i < candidate_count; i++)
{
    // eliminate every candidate who has the minimum votes
    if (!candidates[i].eliminated && candidates[i].votes == min)
    {
        candidates[i].eliminated = true;
    }
}
