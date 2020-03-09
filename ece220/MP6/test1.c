// #include <stdlib.h>
// #include <stdio.h>
// #include "updateBoard.h"
// #include "updateBoardGold.h"
//
// int main(){
// 	int row,col,step;
// 	step = 0;
// 	row = 5;
// 	col = 5;
// 	int i,j;
// 	int game_board_stu[5*5] ={0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0};
// 	int game_board_gold[5*5] = {0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0};
// 	int stuCount,goldCount;
// 	for(i=0;i<row;i++){
// 		for(j=0;j<col;j++){
// 			stuCount = countLiveNeighbor(game_board_stu, row, col, i, j);
// 			goldCount = countLiveNeighbor_gold(game_board_gold,row,col,i,j);
// 			if(stuCount != goldCount){
// 				printf("countLiveNeighbor test failed\n");
// 				return 0;
// 			}
// 		}
// 	}
// 	printf("countLiveNeighbor test passed\n");
// 	step = 0;
// 	while(!aliveStable_gold(game_board_gold, row, col)){
// 		if(step > 10000){
// 			printf("updateBoard test passed\n");
// 			printf("aliveStable test not finished due to too many steps taken\n");
// 			return 0;
// 		}
// 		if(aliveStable(game_board_stu,row,col)){
// 			printf("aliveStable test failed\n");
// 			return 0;
// 		}
// 		updateBoard_gold(game_board_gold,row,col);
// 		updateBoard(game_board_stu,row,col);
// 		for(i=0;i<row;i++){
// 			for(j=0;j<col;j++){
// 				if(game_board_stu[i*col+j]!= game_board_gold[i*col+j]){
// 					printf("updateBoard test failed\n");
// 					return 0;
// 				}
// 			}
// 		}
// 	}
// 	if(!aliveStable(game_board_stu,row,col)){
// 		printf("aliveStable test failed\n");
// 		return 0;
// 	}
// 	printf("updateBoard test passed\n");
// 	printf("aliveStable test passed\n");
// 	return 0;
// }




#include <stdio.h>
#include <stdlib.h>

int col=5,row=5;
int board[25]=
{0,0,0,0,0,
0,0,1,0,0,
0,1,1,1,0,
0,0,1,0,0,
0,0,0,0,0};


int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
	int cell=(row*boardColSize)+col;
	int sum=0;
	if (row==0){				// top row
		if (col==0){			// top left corner
			sum=board[cell+1]+board[cell+boardColSize]+board[cell+boardColSize+1];
		}
		else if (col==boardColSize-1){	// top right corner
			sum=board[cell-1]+board[cell+boardColSize]+board[cell+boardColSize-1];
		}
		else{
			sum=board[cell-1]+board[cell+1]+board[cell+boardColSize-1]+board[cell+boardColSize]+board[cell+boardColSize+1];
		}
	}
	else if (row==boardRowSize-1){
		if (col==0){			// bottom left corner
			sum=board[cell+1]+board[cell-boardColSize]+board[cell-boardColSize+1];
		}
		else if (col==boardColSize-1){	// bottom right corner
			sum=board[cell-1]+board[cell-boardColSize]+board[cell-boardColSize-1];
		}
		else {
			sum=board[cell-1]+board[cell+1]+board[cell-boardColSize-1]+board[cell-boardColSize]+board[cell-boardColSize+1];
		}
	}
	else if (col==0){
		sum=board[cell-boardColSize]+board[cell-boardColSize+1]+board[cell+1]+board[cell+boardColSize]+board[cell+boardColSize+1];
	}
	else if (col==boardColSize-1){
		sum=board[cell-boardColSize]+board[cell-boardColSize-1]+board[cell-1]+board[cell+boardColSize]+board[cell+boardColSize-1];
	}
	else{
		sum=board[cell-boardColSize-1]+board[cell-boardColSize]+board[cell-boardColSize+1]+
		board[cell-1]+board[cell+1]+board[cell+boardColSize-1]+board[cell+boardColSize]+
		board[cell+boardColSize+1];
	}

	return sum;
}

int aliveStable(int* board, int boardRowSize, int boardColSize){
	int row, col, cell, alive, result;
	for (row=0; row<boardRowSize;row++){
		for (col=0; col<boardColSize;col++){
			cell=(row*boardColSize)+col;
			alive=countLiveNeighbor(board,boardRowSize,boardColSize,row,col);
			printf("%d",alive);
			if (board[cell]==1){
				if (alive==2 || alive==3){
					result=1;
				}
				else {
					result=0;
				}
			}
			else {
				if (alive==3){
					result=0;
				}
				else {
					result=1;
				}
			}

		}
		printf("\n");
	}
	printf("\n");
	for (row=0; row<boardRowSize;row++){
		for (col=0; col<boardColSize;col++){
			cell=(row*boardColSize)+col;
			alive=countLiveNeighbor(board,boardRowSize,boardColSize,row,col);
			if (board[cell]==1){
				if (alive==2 || alive==3){
					result=1;
				}
				else {
					return 0;

				}
			}
			else {
				if (alive!=3){
					result=1;
				}
				else {
					return 0;
				}
			}

		}
		printf("\n");
	}
	return result;
}

int main(){
        int i, j;
        for (i=0;i<row;i++){
                for (j=0;j<col;j++){
                        printf("%d",board[i*col+j]);
                }
                printf("\n");
        }
        printf("\n");
	printf("%d",aliveStable(board,row,col));
	return 1;
}
