// ECE 220 MP6
// Developer - Unnat Ramjiyani
// NetID - unnatr2

// The following program is a combination of three functions. The first function (countLiveNeighbor)
// takes the row and column of the cell as input and returns the number of neighbors alive around
// the cell. The fuction checks if the cell is an edge cell or a corner cell and uses the board array
// to find the alive neighbors around the specified cell.


// Takes input as row and column of cell. Changes rows and columns into the index of the cell.
// Checks for corners, edges or ordinary and returns the number of alive neighbors.

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
	int cell=(row*boardColSize)+col;	// Initialize cell to the index of cell in array
	int sum=0;				// Initialize sum to 0
	if (row==0){				// Check if top row
		if (col==0){			// Check if top left corner (corner cell)
			sum=board[cell+1]+board[cell+boardColSize]+board[cell+boardColSize+1];	// Find alive neighbors
		}
		else if (col==boardColSize-1){	// Check if top right corner (corner cell)
			sum=board[cell-1]+board[cell+boardColSize]+board[cell+boardColSize-1];	// Find alive neighbors
		}
		else{				// Is top row (edge cell)
			sum=board[cell-1]+board[cell+1]+board[cell+boardColSize-1]+board[cell+boardColSize]+board[cell+boardColSize+1];	// Find alive neighbors
		}
	}
	else if (row==boardRowSize-1){		// Check if bottor row
		if (col==0){			// Check if bottom left corner (corner cell)
			sum=board[cell+1]+board[cell-boardColSize]+board[cell-boardColSize+1];	// Find alive neighbors
		}
		else if (col==boardColSize-1){	// Check if bottom right corner (corner cell)
			sum=board[cell-1]+board[cell-boardColSize]+board[cell-boardColSize-1];	// Find alive neighbors
		}
		else {				// Is bottom row (edge cell)
			sum=board[cell-1]+board[cell+1]+board[cell-boardColSize-1]+board[cell-boardColSize]+board[cell-boardColSize+1];	// Find alive neighbors
		}
	}
	else if (col==0){			// Check if leftmost column (edge cell)
		sum=board[cell-boardColSize]+board[cell-boardColSize+1]+board[cell+1]+board[cell+boardColSize]+board[cell+boardColSize+1];	// Find alive neighbors
	}
	else if (col==boardColSize-1){		// Check if rightmost column (edge cell)
		sum=board[cell-boardColSize]+board[cell-boardColSize-1]+board[cell-1]+board[cell+boardColSize]+board[cell+boardColSize-1];	// Find alive neighbors
	}
	else{					// Is ordinary cell
		sum=board[cell-boardColSize-1]+board[cell-boardColSize]+board[cell-boardColSize+1]+
		board[cell-1]+board[cell+1]+board[cell+boardColSize-1]+board[cell+boardColSize]+
		board[cell+boardColSize+1];								// Find alive neighbors
	}

	return sum;				// Return alive neighbors
}


// Checks the number of alive neighbors around all cells on the board. If cell is currently alive
// and has 2 or 3 alive neighbors, it will remain alive. If cell is currently alive but has less
// than 2 or more than 3 alive neighbors, it will die. If cell is currently dead and has only 3 alive
// neighbors, it will turb back to alive.

void updateBoard(int* board, int boardRowSize, int boardColSize) {
	int i, j, cell, alive, k;				// Declare variables
	int boardnew[boardRowSize*boardColSize];		// Declare array to save next generation
	for (i=0;i<boardRowSize;i++){				// Loop through all rows
		for (j=0;j<boardColSize;j++){			// Loop through all columns
			cell=(i*boardColSize)+j;		// Initialize cell to the index of cell in board
			alive=countLiveNeighbor(board, boardRowSize, boardColSize, i, j);	// Check number of alive neighbors of current cell
			if (board[cell]==1){			// If cell is alive
				if (alive<2 || alive>3){	// If has less than 2 or more than 3 alive neighbors
					boardnew[cell]=0;	// Cell dies in next generation
				}
				else {				// If has 2 or 3 alive neighbors
					boardnew[cell]=1;	// Cell lives in next generation
				}
			}
			else {					// If cell is dead
				if (alive==3){			// If has 3 alive neighbors
					boardnew[cell]=1;	// Cell lives in next generation
				}
				else {				// If has less than or more than 3 alive neighbors
					boardnew[cell]=0;	// Cell remains dead in next generation
				}
			}
		}
	}
	for (k=0;k<(boardRowSize*boardColSize);k++){		// Loop through all indices of board
		board[k]=boardnew[k];				// Update board array with next generation
	}
}


// Checks if the next generation will change or not. Returns 1 is same or 0 if different.

int aliveStable(int* board, int boardRowSize, int boardColSize){
	int row, col, cell, alive, result;			// Declare variables
	for (row=0; row<boardRowSize;row++){			// Loop through all rows
		for (col=0; col<boardColSize;col++){		// Loop through all columns
			cell=(row*boardColSize)+col;		// Initialize cell to the index of cell in board
			alive=countLiveNeighbor(board,boardRowSize,boardColSize,row,col);	// Check number of alive neighbors of current cell
			if (board[cell]==1){			// If cell is alive
				if (alive==2 || alive==3){	// If has 2 or 3 alive neighbors
					result=1;		// Save result as 1
				}
				else {				// If has less than 2 or more than 3 alive neighbors
					return 0;		// Return 0 (next generation is different)
				}
			}
			else {					// If cell is dead
				if (alive==3){			// If has 3 alive neighbors
					return 0;		// Return 0 (next generation is different)
				}
				else {				// If has less than or more than 3 alive neighbors
					result=1;		// Save result as 1
				}
			}

		}
	}
	return result;						// Return result
}
