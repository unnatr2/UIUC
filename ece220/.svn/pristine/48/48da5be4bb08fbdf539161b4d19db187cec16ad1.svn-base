#include <stdlib.h>
#include <stdio.h>

/* 
 * Your code is included as part of this file.
 */
#include "prog5.h"



//Runs some unit tests for each function
//Indicates pass or fail 
//Input argument should indicate set_seed (0), start_game (1) or make_guess (2)
int main (int argc, char *argv[] )
{
    if (argc == 1){
	printf("Need test No, 0 for set_seed, 1 for start_game, 2 for make_guess\n");
	return 0;
    }
    int command;
    sscanf(argv[1],"%d",&command);
    //Test set seed
    if (command == 0){
	printf("*****Testing set_seed:\n");
	int ret = set_seed("what?");
	int ret2 = set_seed("12321");
	if(ret==0 && ret2==1){
		printf("*****set_seed return value: PASS\n");
	}else{
		printf("*****set_seed return value: FAIL\n");
	}
        //Check if random number generation correct
	set_seed("12325");
        int test = rand();
	if (test == 18534142){
		printf("*****set_seed call srand: PASS\n");
	}else{
		printf("*****set_seed call srand: FAIL\n");
	}
    }
    //Test start game
    if (command == 1){
	printf("*****Testing start_game:\n");
	int one,two,three,four;
	srand(12321);
	start_game(&one,&two,&three,&four);
	int test=rand();
	if (test == 1385017724){
		printf("*****start_game rand() calls: PASS\n");
	}else{
		printf("*****start_game rand() calls: FAIL\n");
        }
	if (one == 8 && two == 4 && three == 8 && four == 8){
		printf("*****start_game ouput: PASS\n");
	}else{
		printf("*****start_game output: FAIL\n");
	}
    }
    //Test Make guess
    //Test return value     
    //Test two different guesses
    if (command == 2){
	printf("*****Testing make_guess output:\n");
	int one,two,three,four;
	srand(12321);
	start_game(&one,&two,&three,&four);
	int ret = make_guess("blahblah",&one,&two,&three,&four);
	int ret2 = make_guess("1 2 3 4",&one,&two,&three,&four);
	if (ret==0 && ret2==1){
		printf("*****make_guess return value: PASS \n");
	}else{ 
		printf("*****make_guess return value: FAIL \n");
        }
	ret = make_guess("8 4 8 8",&one,&two,&three,&four);
	if (one==8 && two==4 && three==8 && four==8){
		printf("*****make_guess output value: PASS \n");
	}else{
		printf("*****make_guess output value: FAIL \n");	
	}
	printf("*****Testing make_guess perfect matches\n");
	printf("*****If next line shows 4 perfect matches, then PASS\n");	
	srand(12321);
	start_game(&one,&two,&three,&four);
	make_guess("8 4 8 8",&one,&two,&three,&four);
	printf("*****Testing make_guess misplaced matches\n");
	printf("*****If next line shows 0 perfect and 2 misplaced matches, then PASS\n");	
	srand(12321);
	start_game(&one,&two,&three,&four);
	make_guess("1 8 4 1",&one,&two,&three,&four);
    }
    return 0;
}

