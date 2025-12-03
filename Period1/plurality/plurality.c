#include <cs50.h>       // get_int(), get_string(), string
#include <stdio.h>      // printf()
#include <string.h>     // strcmp() for comparing candidate names. Checks if they are really identical


#define MAX 9   //Max 9 Candidates


typedef struct
{
    string name;    //give string a name
    int votes;      // name for votes
} candidate;        //name it


candidate candidates[MAX]; //global array


int candidate_count;    //how many candidates


bool vote(string name);     //function for main
void print_winner(void);    //function for main

int main(int argc, string argv[]) //program start
{

    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;//At least one Candidate name
    }

    candidate_count = argc - 1; //number of candidates - 1
    if (candidate_count > MAX) //too many candidates?
    {
        printf("Maximum number of candidates is %i\n", MAX); //complain like always
        return 2; //different error code
    }
    for (int i = 0; i < candidate_count; i++) //initialize each candidate row
    {
        candidates[i].name = argv[i + 1]; //start with candidate 1 argv normally 0 and this sets it to alice, Bob or charlie, depends on i
        candidates[i].votes = 0; //set the votes to 0
    }

    int voter_count = get_int("Number of voters: "); //ask how many voters


    for (int i = 0; i < voter_count; i++) //handle each voter one time
    {
        string name = get_string("Vote: "); // read the voted name as text


        if (!vote(name)) //try to record it. !=not
        {
            printf("Invalid vote.\n"); //fail if name not right
        }
    }


    print_winner(); // guess
}


bool vote(string name)                           // record one vote by name
{
    for (int i = 0; i < candidate_count; i++)    // scan all candidates
    {
        if (strcmp(candidates[i].name, name) == 0) // exact name match?
        {
            candidates[i].votes++;               // add one vote to that candidate
            return true;                         // success
        }
    }
    return false;                                // no match → invalid vote
}




void print_winner(void)                          // output winner(s)
{
    int max_votes = 0;                           // track the highest vote count seen
    for (int i = 0; i < candidate_count; i++)    // first pass: find maximum
    {
        if (candidates[i].votes > max_votes)
        {
            max_votes = candidates[i].votes; //change the max votes if necessary
        }
    }

    for (int i = 0; i < candidate_count; i++)    // second pass: print all with that maximum
    {
        if (candidates[i].votes == max_votes)    // handles ties naturally
        {
            printf("%s\n", candidates[i].name);
        }
    }
}
