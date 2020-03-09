// ECE220 MP9
// Developer: Unnat Ramjiyani
// NetID: unnatr2

// The following code executes a program that reads, solves and prints a two
// dimentional maze. The maze is read from a file and the start and end points are
// determined and set. The program then uses depth first search method to find the
// solution to the maze. The program prints the unsolved and the solved maze.

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
        int width, height, col, row;                                    // Declare variables
        char cell_char;                                                 // Declare variables
        FILE *fp;                                                       // Declare file pointer
        maze_t * mygame = malloc(sizeof(maze_t));                       // Allocate memory for maze_t
        fp = fopen(fileName,"r");                                       // Open file
        fscanf(fp,"%i %i",&width,&height);                              // Read width and height from file
        mygame->cells = malloc(height*sizeof(char*));                   // Allocate space for rows
        for (row=0;row<height;row++){                                   // Loop through rows
                (mygame->cells)[row] = malloc(width*sizeof(char));      // Allocate space for columns
        }
        mygame->width=width;                                            // Set member - width
        mygame->height=height;                                          // Set member - height
        fseek(fp,1,SEEK_CUR);                                           // Skip the line
        for (row=0;row<height;row++){                                   // Loop through rows
                for (col=0;col<width;col++){                            // Loop through columns
                        cell_char=fgetc(fp);                            // Get character from file
                        if (cell_char=='\n'){                           // Check if new line character
                                cell_char=fgetc(fp);                    // Get next character from file
                        }
                        mygame->cells[row][col]=cell_char;              // Set cell to value
                }
        }
        fclose(fp);                                                     // Close file
        for (col=0;col<width;col++){                                    // Loop through columns
                for (row=0;row<height;row++){                           // Loop through rows
                        cell_char=mygame->cells[row][col];              // Save value of cell
                        if (cell_char=='S'){                            // Check if start
                                mygame->startRow=row;                   // Set member - startRow
                                mygame->startColumn=col;                // Set member - startColumn
                        }
                        if (cell_char=='E'){                            // Check if end
                                mygame->endRow=row;                     // Set member - endRow
                                mygame->endColumn=col;                  // Set member - endColumn
                        }
                }
        }
        return mygame;                                                  // Return pointer to maze
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
        int i, height=maze->height;                                     // Declare variables
        for (i=0;i<height;i++){                                         // Loop through rows
                free(maze->cells[i]);                                   // Free all columns
        }
        free(maze->cells);                                              // Free all rows
        free(maze);                                                     // Free maze
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    int row, col, width=maze->width, height=maze->height;               // Declare variables
    for (row=0;row<height;row++){                                       // Loop through rows
            for (col=0;col<width;col++){                                // Loop through columns
                    printf("%c",maze->cells[row][col]);                 // Print current cell
            }
            printf("\n");                                               // Print new line
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */
int solveMazeManhattanDFS(maze_t * maze, int col, int row)
{
        int height=maze->height, width=maze->width;                     // Declare variables
        if (col<0 || col>=width || row<0 || row>=height){               // Check if row and column are valid
                return 0;                                               // Return 0
        }
        if (maze->cells[row][col]!=' ' && maze->cells[row][col]!='S' && maze->cells[row][col]!='E'){    // Check if cell is invalid
                return 0;                                               // Return 0
        }
        if (maze->cells[row][col]=='E'){                                // Check if reached end
                maze->cells[maze->startRow][maze->startColumn]='S';     // Set value of start cell to 'S'
                return 1;                                               // Return 1
        }
        maze->cells[row][col]='*';                                      // Set cell to solution path
        if (solveMazeManhattanDFS(maze,col-1,row)==1){                  // Check if left adjacent cell is solution
                return 1;                                               // Return 1
        }
        if (solveMazeManhattanDFS(maze,col,row+1)==1){                  // Check if bottom adjacent cell is solution
                return 1;                                               // Return 1
        }
        if (solveMazeManhattanDFS(maze,col+1,row)==1){                  // Check if right adjacent cell is solution
                return 1;                                               // Return 1
        }
        if (solveMazeManhattanDFS(maze,col,row-1)==1){                  // Check if top adjacent cell is solution
                return 1;                                               // Return 1
        }
        maze->cells[row][col]='~';                                      // Set cell to visited
        return 0;                                                       // Return 0
}
