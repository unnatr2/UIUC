// ECE220 MP7
// Developer: Unnat Ramjiyani
// NetID: unnatr2
//
// The following code solves a sudoku board using recursion. The code checks the board
// and finds an empty cell, then loops through all possible values for that cell to
// find the correct value. This is done using recursion.

#include "sudoku.h"

// The following function checks if the value passed is present in the row of the sudoku.
// It returns 1 if true or 0 if false.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {
        assert(i>=0 && i<9);                    // Check if row is between 0 and 9
        int j;                                  // Declare variable j
        for (j=0;j<9;j++){                      // Loop through all columns
                if (sudoku[i][j]==val){         // Check if value is in any cell
                        return true;            // If yes, return true
                }
        }
        return false;                           // Else, return false
}

// The following function checks if the value passed is present in the column of the sudoku.
// It returns 1 if true or 0 if false.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {
        assert(j>=0 && j<9);                    // Check if column is between 0 and 9
        int i;                                  // Declare variable i
        for (i=0;i<9;i++){                      // Loop through all rows
                if (sudoku[i][j]==val){         // Check if value is in any cell
                        return true;            // If yes, return true
                }
        }
        return false;                           // Else, return false
}

// The following function checks if the value passed is present in the 3x3 box of the sudoku.
// It returns 1 if true or 0 if false.
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
        assert(i>=0 && i<9 && j>=0 && j<9);     // Check if row and column is between 0 and 9
        int rowset, colset, row, col;           // Declare variables
        rowset=i%3;                             // Find how far right from left edge of the box
        colset=j%3;                             // Find how far down from the top edge of the box
        for (row=0;row<3;row++){                // Loop through all rows of box
                for (col=0;col<3;col++){        // Loop through all columns of box
                        if (sudoku[(i-rowset)+row][(j-colset)+col]==val){       // Check if box has value
                                return true;    // If yes, return true
                        }
                }
        }
        return false;                           // Else, return false
}

// The following function checks if the value passed is valid for the cell.
// It returns 1 if true or 0 if false.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {
        assert(i>=0 && i<9 && j>=0 && j<9);     // Check if row and column is between 0 and 9
        if (is_val_in_row(val,i,sudoku)==0 && is_val_in_col(val,j,sudoku)==0 && is_val_in_3x3_zone(val,i,j,sudoku)==0){ // Check if value is not present in row, column and box
                return true;                    // If yes, return true
        }
        return false;                           // Else, return false
}

// The following function loops through all cells to find if a cell is empty then
// uses recursion to find which value can be put in the cell.
// Returns true if solved, false if cannot be solved.
int solve_sudoku(int sudoku[9][9]) {
        int i, j, num, result;                          // Declare variables
        for (i=0;i<9;i++){                              // Loop through rows
                for (j=0;j<9;j++){                      // Loop through columns
                        if (sudoku[i][j]>0 && sudoku[i][j]<10){ // Check if cell has value
                                result=1;               // If true, result = 1
                        }
                        else {
                                result=0;               // Else, result = 0
                                goto out1;              // Jump out of loop
                        }
                }
        }
        out1:                                           // Label for outloop
        if (result==1){                                 // Check if result = 1
                return true;                            // If yes, return true
        }
        else {                                          // If no
                for (i=0;i<9;i++){                      // Loop through rows
                        for (j=0;j<9;j++){              // Loop through columns
                                if (sudoku[i][j]==0){   // Find empty cell
                                        goto out2;      // Jump out of loop
                                }
                        }
                }
        }
        out2:                                           // Label for outloop
        for (num=1;num<=9;num++){                       // Loop through values between 1 and 9
                if (is_val_valid(num,i,j,sudoku)==1){   // Check if value is valid
                        sudoku[i][j]=num;               // If valid, put value in cell
                        if (solve_sudoku(sudoku)==1){   // Check if sudoku is solved
                                return true;            // If yes, return true
                        }
                        sudoku[i][j]=0;                 // If not, remove value from cell
                }
        }
        return false;                                   // Return false
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
