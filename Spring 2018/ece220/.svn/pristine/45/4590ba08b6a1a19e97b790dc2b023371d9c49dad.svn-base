// ECE220 MP8
// Developer: Unnat Ramjiyani
// NetID: unnatr2

// The following code executes a game of 2048. The user inputs the number of rows and columns
// of the game board and using the keys (w, a, s and d), the user can shift the cells on the
// game board in the desired direction. If two adjacent cells have the same value, they merge
// and the merged cell gets the value of the sum of the two previous cells. The goal is to
// reach 2048. The score updates by the addition of the value of the merged cell.

#include "game.h"


game * make_game(int rows, int cols)
/*!      Create an instance of a game structure with the given number of rows
        and columns, initializing elements to -1 and return a pointer
        to it. (See game.h for the specification for the game data structure)
        The needed memory should be dynamically allocated with the malloc family
        of functions.
*/
{
        //Dynamically allocate memory for game and cells (DO NOT modify this)
        game * mygame = malloc(sizeof(game));
        mygame->cells = malloc(rows*cols*sizeof(cell));

        mygame->rows=rows;                      // Set rows of the game to rows
        mygame->cols=cols;                      // Set columns of the game to cols
        mygame->score=0;                        // Set score of the game to 0
        int i;
        cell *cell_pointer;
        cell_pointer=mygame->cells;             // Pointer to array cells
        for (i=0;i<rows*cols;i++){              // Loop through array cells
                *cell_pointer=-1;               // Set every index value to -1
                cell_pointer++;
        }
        return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*!     Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any
	memory previously allocated is not lost in this function.
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

        (*_cur_game_ptr)->rows=new_rows;        // Set rows of the game to new_rows
        (*_cur_game_ptr)->cols=new_cols;        // Set columns of the game to new_cols
        (*_cur_game_ptr)->score=0;              // Set score of the game to 0
        int i;
        cell *cell_pointer;
        cell_pointer=(*_cur_game_ptr)->cells;   // Pointer to array cells
        for (i=0;i<new_rows*new_cols;i++){      // Loop through array cells
                *cell_pointer=-1;               // Set every index value to -1
                cell_pointer++;
        }
	return;
}

void destroy_game(game * cur_game)
/*!      Deallocate any memory acquired with malloc associated with the given game instance.
        This includes any substructures the game data structure contains. Do not modify this function.*/
{
        free(cur_game->cells);
        free(cur_game);
        cur_game = NULL;
        return;
}

cell * get_cell(game * cur_game, int row, int col)
/*!     Given a game, a row, and a column, return a pointer to the corresponding
        cell on the game. (See game.h for game data structure specification)
        This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
        if (row<cur_game->rows && col<cur_game->cols && row>=0 && col>=0){      // Check if row and col is valid
                return &cur_game->cells[row*cur_game->cols+col];                // Return pointer to cell
        }
        return NULL;
}

int move_w(game * cur_game)
/*!     Slides all of the tiles in cur_game upwards. If a tile matches with the
        one above it, the tiles are merged by adding their values together. When
        tiles merge, increase the score by the value of the new tile. A tile can
        not merge twice in one turn. If sliding the tiles up does not cause any
        cell to change value, w is an invalid move and return 0. Otherwise, return 1.
*/
{
        int row,col,last_combined_row,t_row,flag=0;
        int rowset=cur_game->rows,colset=cur_game->cols;

        for (col=0;col<colset;col++){                                                   // Loop through all columns
                last_combined_row=-1;                                                   // Initialize last_combined_row
                for (row=0;row<rowset;row++){                                           // Loop though all rows
                        if (*get_cell(cur_game,row,col)!=-1){                           // Check if not empty
                                t_row=0;
                                for (t_row=0;t_row<row;t_row++){                        // Find target row smaller than row
                                        if (*get_cell(cur_game,t_row,col)==-1){         // Check if empty
                                                break;                                  // End loop
                                        }
                                }
                                if (t_row<row){
                                        *get_cell(cur_game,t_row,col)=*get_cell(cur_game,row,col);      // Target cell gets value of original cell
                                        *get_cell(cur_game,row,col)=-1;                 // Original cell set to empty
                                        flag=1;                                         // Game board changed
                                }
                                if (t_row-1!=last_combined_row){                        // Check if row above target row is valid for merger
                                        if (*get_cell(cur_game,t_row-1,col)==*get_cell(cur_game,t_row,col) && t_row>=0){        // Check if equal
                                                *get_cell(cur_game,t_row-1,col)=*get_cell(cur_game,t_row-1,col)+*get_cell(cur_game,t_row,col);  // Set merged cell to sum
                                                *get_cell(cur_game,t_row,col)=-1;       // Target cell set to empty
                                                last_combined_row=t_row-1;              // Update last_combined_row
                                                cur_game->score+=*get_cell(cur_game,t_row-1,col);       // Update score
                                                flag=1;                                 // Game board changed
                                        }
                                }
                        }
                }
        }
        if (flag==1){
                return 1;
        }
        return 0;
}

int move_s(game * cur_game) //slide down
{
        int row,col,last_combined_row,t_row,flag=0;
        int rowset=cur_game->rows,colset=cur_game->cols;

        for (col=0;col<colset;col++){                                                   // Loop through all columns
                last_combined_row=rowset;                                               // Initialize last_combined_row
                for (row=rowset-1;row>=0;row--){                                        // Loop though all rows
                        if (*get_cell(cur_game,row,col)!=-1){                           // Check if not empty
                                t_row=rowset-1;
                                for (t_row=rowset-1;t_row>row;t_row--){                 // Find target row greater than row
                                        if (*get_cell(cur_game,t_row,col)==-1){         // Check if empty
                                                break;                                  // End loop
                                        }
                                }
                                if (t_row>row){
                                        *get_cell(cur_game,t_row,col)=*get_cell(cur_game,row,col);      // Target cell gets value of original cell
                                        *get_cell(cur_game,row,col)=-1;                 // Original cell set to empty
                                        flag=1;                                         // Game board changed
                                }
                                if (t_row+1!=last_combined_row){                        // Check if row below target row is valid for merger
                                        if (*get_cell(cur_game,t_row+1,col)==*get_cell(cur_game,t_row,col) && t_row<rowset){    // Check if equal
                                                *get_cell(cur_game,t_row+1,col)=*get_cell(cur_game,t_row+1,col)+*get_cell(cur_game,t_row,col);  // Set merged cell to sum
                                                *get_cell(cur_game,t_row,col)=-1;       // Target cell set to empty
                                                last_combined_row=t_row+1;              // Update last_combined_row
                                                cur_game->score+=*get_cell(cur_game,t_row+1,col);       // Update score
                                                flag=1;                                 // Game board changed
                                        }
                                }
                        }
                }
        }
        if (flag==1){
                return 1;
        }
        return 0;
}

int move_a(game * cur_game) //slide left
{
        int row,col,last_combined_col,t_col,flag=0;
        int rowset=cur_game->rows,colset=cur_game->cols;

        for (row=0;row<rowset;row++){                                                   // Loop though all rows
                last_combined_col=-1;                                                   // Initialize last_combined_col
                for (col=0;col<colset;col++){                                           // Loop through all columns
                        if (*get_cell(cur_game,row,col)!=-1){                           // Check if not empty
                                t_col=0;
                                for (t_col=0;t_col<col;t_col++){                        // Find target column smaller than col
                                        if (*get_cell(cur_game,row,t_col)==-1){         // Check if empty
                                                break;                                  // End loop
                                        }
                                }
                                if (t_col<col){
                                        *get_cell(cur_game,row,t_col)=*get_cell(cur_game,row,col);      // Target cell gets value of original cell
                                        *get_cell(cur_game,row,col)=-1;                 // Original cell set to empty
                                        flag=1;                                         // Game board changed
                                }
                                if (t_col-1!=last_combined_col){                        // Check if column before target column is valid for merger
                                        if (*get_cell(cur_game,row,t_col-1)==*get_cell(cur_game,row,t_col) && t_col>=0){        // Check if equal
                                                *get_cell(cur_game,row,t_col-1)=*get_cell(cur_game,row,t_col-1)+*get_cell(cur_game,row,t_col);  // Set merged cell to sum
                                                *get_cell(cur_game,row,t_col)=-1;       // Target cell set to empty
                                                last_combined_col=t_col-1;              // Update last_combined_col
                                                cur_game->score+=*get_cell(cur_game,row,t_col-1);       // Update score
                                                flag=1;                                 // Game board changed
                                        }
                                }
                        }
                }
        }
        if (flag==1){
                return 1;
        }
        return 0;
}

int move_d(game * cur_game) //slide to the right
{
        int row,col,last_combined_col,t_col,flag=0;
        int rowset=cur_game->rows,colset=cur_game->cols;

        for (row=0;row<rowset;row++){                                                   // Loop though all rows
                last_combined_col=colset;                                               // Initialize last_combined_col
                for (col=colset-1;col>=0;col--){                                        // Loop through all columns
                        if (*get_cell(cur_game,row,col)!=-1){                           // Check if not empty
                                t_col=colset-1;
                                for (t_col=colset-1;t_col>col;t_col--){                 // Find target column greater than col
                                        if (*get_cell(cur_game,row,t_col)==-1){         // Check if empty
                                                break;                                  // End loop
                                        }
                                }
                                if (t_col>col){
                                        *get_cell(cur_game,row,t_col)=*get_cell(cur_game,row,col);      // Target cell gets value of original cell
                                        *get_cell(cur_game,row,col)=-1;                 // Original cell set to empty
                                        flag=1;                                         // Game board changed
                                }
                                if (t_col+1!=last_combined_col){                        // Check if column after target column is valid for merger
                                        if (*get_cell(cur_game,row,t_col+1)==*get_cell(cur_game,row,t_col) && t_col<colset){    // Check if equal
                                                *get_cell(cur_game,row,t_col+1)=*get_cell(cur_game,row,t_col+1)+*get_cell(cur_game,row,t_col);  // Set merged cell to sum
                                                *get_cell(cur_game,row,t_col)=-1;       // Target cell set to empty
                                                last_combined_col=t_col+1;              // Update last_combined_col
                                                cur_game->score+=*get_cell(cur_game,row,t_col+1);       // Update score
                                                flag=1;                                 // Game board changed
                                        }
                                }
                        }
                }
        }
        if (flag==1){
                return 1;
        }
        return 0;
}

int legal_move_check(game * cur_game)
/*!     Given the current game check if there are any legal moves on the board. There are
        no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
        int i,row,col;
        int rowset=cur_game->rows,colset=cur_game->cols;
        for (i=0;i<rowset*colset;i++){                          // Loop through all cells
                if (cur_game->cells[i]==-1){                    // Check if empty
                        return 1;
                }
        }
        for (row=0;row<rowset;row++){                           // Loop through all rows
                for (col=0;col<colset;col++){                   // Loop through all columns
                        if (row==0){                            // Top row
                                if (col==0){                    // Top left corner
                                        if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row,col+1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row+1,col)){
                                                return 1;
                                        }
                                }
                                else if (col==colset-1){        // Top right corner
                                        if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row,col-1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row+1,col)){
                                                return 1;
                                        }
                                }
                                else {                          // Top row
                                        if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row,col-1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row+1,col) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row,col+1)){
                                                return 1;
                                        }
                                }
                        }
                        else if (row==rowset-1){                // Bottom row
                                if (col==0){                    // Bottom left corner
                                        if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row,col+1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row-1,col)){
                                                return 1;
                                        }
                                }
                                else if (col==colset-1){        // Bottom right corner
                                        if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row,col-1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row-1,col)){
                                                return 1;
                                        }
                                }
                                else {                          // Bottom row
                                        if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row,col-1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row-1,col) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row,col+1)){
                                                return 1;
                                        }
                                }
                        }
                        else if (col==0){                       // Left column
                                if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row-1,col) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row,col+1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row+1,col)){
                                        return 1;
                                }
                        }
                        else if (col==colset-1){                // Right column
                                if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row-1,col) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row,col-1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row+1,col)){
                                        return 1;
                                }
                        }
                        else {                                  // Ordinary cell
                                if (*get_cell(cur_game,row,col)==*get_cell(cur_game,row-1,col) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row,col-1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row,col+1) || *get_cell(cur_game,row,col)==*get_cell(cur_game,row+1,col)){
                                        return 1;
                                }
                        }
                }
        }
        return 0;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*!     insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	cell * cell_ptr;
        cell_ptr = 	cur_game->cells;

        if (cell_ptr == NULL){
        printf("Bad Cell Pointer.\n");
        exit(0);
        }


	//check for an empty cell
	int emptycheck = 0;
	int i;

	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
			emptycheck = 1;
			break;
		}
                cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}

        int ind,row,col;
	int num;
        do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
        } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game)
{
        cell * cell_ptr;
        cell_ptr = 	cur_game->cells;

        int rows = cur_game->rows;
        int cols = cur_game->cols;
        int i,j;

	printf("\n\n\nscore:%d\n",cur_game->score);


	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char


        for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
                for(j = 0; j < cols; j++){
                        if ((*cell_ptr) == -1 ) { //print asterisks
                                printf(" **  ");
                        }
                        else {
                                switch( *cell_ptr ){ //print colored text
                                        case 2:
                                                printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 4:
                                                printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 8:
                                                printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 16:
                                                printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 32:
                                                printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 64:
                                                printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 128:
                                                printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 256:
                                                printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 512:
                                                printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 1024:
                                                printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 2048:
                                                printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 4096:
                                                printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                                                break;
                                        case 8192:
                                                printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                                                break;
        				default:
        					printf("  X  ");

                                }

                        }
                        cell_ptr++;
                }
	        printf("\u2551\n"); //print right wall and newline
        }

	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char

        return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{
	int rows,cols;
	char buf[200];
	char garbage[2];
        int move_success = 0;

        switch ( input_char ) {
                case 'w':
                        move_success = move_w(cur_game);
                        break;
                case 'a':
                        move_success = move_a(cur_game);
                        break;
                case 's':
                        move_success = move_s(cur_game);
                        break;
                case 'd':
                        move_success = move_d(cur_game);
                        break;
                case 'q':
                        destroy_game(cur_game);
                        printf("\nQuitting..\n");
                        return 0;
                        break;
	        case 'n':
		        //get row and col input for new game
		        dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		        while (NULL == fgets(buf,200,stdin)) {
			        printf("\nProgram Terminated.\n");
			        return 0;
		        }

		        if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) || rows < 0 || cols < 0){
			        printf("Invalid dimensions.\n");
			        goto dim_prompt;
		        }

		        remake_game(&cur_game,rows,cols);

		        move_success = 1;

                default: //any other input
                        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
        }


        if(move_success == 1){ //if movement happened, insert new tile and print the game.
                rand_new_tile(cur_game);
		print_game(cur_game);
        }

        if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
                printf("Game Over!\n");
                return 0;
        }
        return 1;
}
