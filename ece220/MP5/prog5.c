// ECE220 MP5
// Developer - Unnat Ramjiyani
// NetID - unnatr2

// The following code runs a game similar to Mastermind in which the user inputs
// a seed value. This seed value is used to generate 4 random integers between 1
// and 8 that serve as the solution code. The user is then prompted to enter their
// guess and is displayed with the number of perfect matches and the number of
// misplaced matches. The user has 12 tries to guess the random solution and if they
// fail, the solution is printed on the screen.

#include <stdio.h>
#include <stdlib.h>

// Inputs the header file prog5.h into the code

#include "prog5.h"

// Static variables

static int guess_number;        // Keeps track of the number of guesses made
static int solution1;           // Saves the first number of the solution
static int solution2;           // Saves the second number of the solution
static int solution3;           // Saves the third number of the solution
static int solution4;           // Saves the fourth number of the solution
static int perf, mis;           // Keeps track of the number of perfect and misplaced matches

// The following function takes in the seed string from the user, checks if the
// string is valid and then uses the seed to initialize the random funtions.

int set_seed (const char seed_str[])
{
        int seed,result;                                // Local variables
        char post[2];                                   // Array that stores characters
        result=sscanf(seed_str,"%d%1s",&seed, post);    // Split seed_str into int and char
        if (result==0||result==2){                      // Check if invalid
                printf("set_seed: invalid seed\n");     // Print INVALID statement
                return 0;                               // Return 0
        }
        else {                                          // If valid
                srand (seed);                           // Set srand using seed
                return 1;                               // Return 1
        }
}

// The following function starts the game by generating the random solutions. The
// rand function outputs long integers but can be constraint between 1 and 8 using
// modulus operator.

void start_game (int* one, int* two, int* three, int* four)
{
        solution1=(rand()%8)+1;         // Save solution1
        solution2=(rand()%8)+1;         // Save solution2
        solution3=(rand()%8)+1;         // Save solution3
        solution4=(rand()%8)+1;         // Save solution4
        *one=solution1;                 // Set pointer to solution1
        *two=solution2;                 // Set pointer to solution2
        *three=solution3;               // Set pointer to solution3
        *four=solution4;                // Set pointer to solution4
        guess_number=1;                 // Initialize guess_number to 1
}

// The following function counts the number of perfect matches and the number of
// misplaced matches.

void check_guess (int w, int x, int y, int z)
{
        int countperf=0,countmis=0;                     // Local variables to save count
        int i,j;                                        // Local variables for iterating
        int pairg[4], pairs[4], guess[4], ans[4];       // Arrays to itterate
        ans[0]=solution1;                               // Save solution1 in array ans
        ans[1]=solution2;                               // Save solution2 in array ans
        ans[2]=solution3;                               // Save solution3 in array ans
        ans[3]=solution4;                               // Save solution4 in array ans
        guess[0]=w;                                     // Save guess1(w) in array guess
        guess[1]=x;                                     // Save guess2(x) in array guess
        guess[2]=y;                                     // Save guess3(y) in array guess
        guess[3]=z;                                     // Save guess4(z) in array guess
        for (i=0;i<=3;i++){                             // Loop 4 times
                if (guess[i]==ans[i]){                  // Check if perfect match
                        pairg[i]=1;                     // Set guess[i] as paired
                        pairs[i]=1;                     // Set solution[i] as paired
                        countperf++;                    // Increment perfect match counter
                }
                else{                                   // If not perfect match
                        pairg[i]=0;                     // Set guess[i] as unpaired
                        pairs[i]=0;                     // Set solution[i] as unpaired
                }
        }
        for (i=0;i<=3;i++){                             // Loop 4 times
                for (j=0;j<=3;j++){                     // Loop 4 times
                        if (i!=j && pairg[i]!=1 && pairs[j]!=1 && guess[i]==ans[j]){    // Check if misplaced match
                                pairg[i]=1;             // Set guess[i] as paired
                                pairs[j]=1;             // Set solution[j] as paired
                                countmis++;             // Increment misplaced match counter
                        }
                }

        }
        perf=countperf;                                 // Save perfect match count
        mis=countmis;                                   // Save misplaced match count
}

// The following function checks if the guess made by the user is valid or not. If
// valid, the function calls check_guess() and checks for perfect and misplaced matches
// and then prints them out. Finally the function also increments the guess_number.
// If invalid, the error message is printed.

int make_guess (const char guess_str[], int* one, int* two, int* three, int* four)
{
        char post[2];                                   // Array to save characters
        int w,x,y,z,result,ans;                         // Local variables
        result=sscanf(guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post);          // Split user input into 4 integers and character array
        if (result==4){                                 // Check if only 4 integers
                ans=1;                                  // Save ans as 1
                if (w>=1&&w<=8 && x>=1&&x<=8 && y>=1&&y<=8 && z>=1&&z<=8){      // Check if the 4 integers are between 1 and 8
                        ans=1;                          // Save ans as 1
                }
                else{                                   // If not between 1 and 8
                        ans=0;                          // Save ans as 0
                }
        }
        else{                                           // If invalid string
                ans=0;                                  // Save ans as 0
        }
        if (ans==1){                                    // If string is valid
                *one=w;                                 // Save guess1 to w
                *two=x;                                 // Save guess2 to x
                *three=y;                               // Save guess3 to y
                *four=z;                                // Save guess4 to z
                check_guess(w,x,y,z);                   // Call check_guess()
                printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n",guess_number,perf,mis);  // Print guess_number, perfect matches and misplaced matches
                guess_number++;                         // Increment guess_number
                return 1;                               // Return 1
        }
        else{                                           // If invalid
                printf("make_guess: invalid guess\n");  // Print INVALID statement
                return 0;                               // Return 0
        }
}
