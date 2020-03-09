// ECE 220 MP10
// Developer: Unnat Ramjiyani
// NetID: unnatr2
//
// The following code creates a sparse matrix using an input file provided. The
// matrix is created using a list of list which allows the creation of useful
// nodes without the creation of nodes that contain zeros. The code also allows the
// user to add two matrices with the same number of rows and columns.

#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

void del_node(sp_lils *mat, int row, int col);

sp_lils * load_lils(char* input_file){
    int total_rows, total_cols;                                 // Declare variables
    int row, col;                                               // Declare variables
    double val;                                                 // Declare variables
    FILE *file_ptr;                                             // Declare file pointer
    sp_lils *mat=malloc(sizeof(sp_lils));                       // Allocate matrix
    file_ptr=fopen(input_file,"r");                             // Set file pointer
    fscanf(file_ptr,"%i %i",&total_rows,&total_cols);           // Get total rows and columns
    mat->m=total_rows;                                          // Set total rows
    mat->n=total_cols;                                          // Set total cols
    mat->nz=0;                                                  // Set non-zero cells
    mat->row_list_head=NULL;                                    // Set row head
    while (fscanf(file_ptr,"%i %i %lf",&row,&col,&val)!=EOF){   // Scan cell row, column and value
        set_lils(mat,row,col,val);                              // Set node in matrix
    }
    fclose(file_ptr);                                           // Close file
    return mat;                                                 // Return matrix
}

double gv_lils(sp_lils * mat_l,int row,int col){
    sp_lils_row_node *row_ptr;                                  // Declare varaibles
    sp_lils_col_node *col_ptr;                                  // Declare varaibles
    row_ptr=mat_l->row_list_head;                               // Set row pointer
    while (row_ptr!=NULL){                                      // Loop through rows
        if (row_ptr->idx_r==row){                               // Check if row is the given row
            col_ptr=row_ptr->col_list_head;                     // Set column pointer
            while (col_ptr!=NULL){                              // Loop through columns
                if (col_ptr->idx_c==col){                       // Check if column is given column
                    return col_ptr->value;                      // Return value of cell
                }
                col_ptr=col_ptr->next;                          // Move to next column
            }
        }
        row_ptr=row_ptr->next;                                  // Move to next row
    }
    return 0;                                                   // Return 0 if cell doesn't exist
}

void set_lils(sp_lils * mat_l, int row, int col, double value){
    sp_lils_row_node *row_ptr, *temp_r, *cur_r;                 // Declare variables
    sp_lils_col_node *col_ptr, *temp_c, *cur_c;                 // Declare varaibles
    if (value==0){                                              // Check if value is 0
        del_node(mat_l,row,col);                                // Delete the node
    }
    else {
        double val=gv_lils(mat_l,row,col);                      // Find value of cell
        if (val!=0){                                            // Check if cell exists
            row_ptr=mat_l->row_list_head;                       // Set row pointer
            while (row_ptr!=NULL){                              // Loop through rows
                if (row_ptr->idx_r==row){                       // Check if row is given row
                    col_ptr=row_ptr->col_list_head;             // Set column pointer
                    while (col_ptr!=NULL){                      // Loop through columns
                        if (col_ptr->idx_c==col){               // Check if column is given columns
                            col_ptr->value=value;               // Set value of cell
                            return;                             // Return
                        }
                        col_ptr=col_ptr->next;                  // Move to next column
                    }
                }
                row_ptr=row_ptr->next;                          // Move to next row
            }
        }
        else {
            while (mat_l->row_list_head==NULL){                 // Check if first ever node
                temp_r=(sp_lils_row_node*)malloc(sizeof(sp_lils_row_node));     // Allocate temporary row node
                temp_c=(sp_lils_col_node*)malloc(sizeof(sp_lils_row_node));     // Allocate temporary column node
                temp_c->idx_c=col;                              // Set index of column
                temp_c->value=value;                            // Set value of cell
                temp_c->next=NULL;                              // Set next of column to NULL
                temp_r->idx_r=row;                              // Set index of row
                temp_r->col_list_head=temp_c;                   // Set column head
                temp_r->next=NULL;                              // Set next of row to NULL
                mat_l->row_list_head=temp_r;                    // Set row head
                mat_l->nz++;                                    // Increment non-zero cells
                return;                                         // Return
            }
            row_ptr=mat_l->row_list_head;                       // Set row pointer
            while (row_ptr!=NULL){                              // Loop through rows
                if (row_ptr->idx_r==row){                       // Check if row is given row
                    col_ptr=row_ptr->col_list_head;             // Set column pointer
                    if (col_ptr!=NULL && col_ptr->idx_c>col){   // Check if given column is smaller than column pointer
                        temp_c=(sp_lils_col_node*)malloc(sizeof(sp_lils_col_node));             // Allocate temporary column node
                        temp_c->idx_c=col;                      // Set index of column
                        temp_c->value=value;                    // Set value of cell
                        temp_c->next=col_ptr;                   // Set next of column to column pointer
                        row_ptr->col_list_head=temp_c;          // Set column head to column
                        mat_l->nz++;                            // Increment non-zero cells
                        return;                                 // Return
                    }
                    while (col_ptr!=NULL && col_ptr->idx_c<col){                // Loop through columns
                        cur_c=col_ptr;                          // Set current column smaller than given column
                        col_ptr=col_ptr->next;                  // Move to next column
                    }
                    temp_c=(sp_lils_col_node*)malloc(sizeof(sp_lils_col_node)); // Allocate temporary column node
                    temp_c->idx_c=col;                          // Set index of column
                    temp_c->value=value;                        // Set value of cell
                    temp_c->next=cur_c->next;                   // Set next of column to next of current column
                    cur_c->next=temp_c;                         // Set next to current column to column
                    mat_l->nz++;                                // Increment non-zero cells
                    return;                                     // Return
                }
                row_ptr=row_ptr->next;                          // Move to next row
            }
            row_ptr=mat_l->row_list_head;                       // Set row pointer
            if (row_ptr!=NULL && row_ptr->idx_r>row){           // Check if given row is smaller than row pointer
                temp_r=(sp_lils_row_node*)malloc(sizeof(sp_lils_row_node));     // Allocate temporary row node
                temp_c=(sp_lils_col_node*)malloc(sizeof(sp_lils_row_node));     // Allocate temporary column node
                temp_c->idx_c=col;                              // Set index of column
                temp_c->value=value;                            // Set value of cells
                temp_c->next=NULL;                              // Set next of column to NULL
                temp_r->idx_r=row;                              // Set index of row
                temp_r->col_list_head=temp_c;                   // Set column head to column
                temp_r->next=row_ptr;                           // Set next of row to row pointer
                mat_l->row_list_head=temp_r;                    // Set row head to row
                mat_l->nz++;                                    // Increment non-zero cells
                return;                                         // Return
            }
            while (row_ptr!=NULL && row_ptr->idx_r<row){        // Loop through rows
                cur_r=row_ptr;                                  // Set current row smaller than given row
                row_ptr=row_ptr->next;                          // Move to next row
            }
            temp_r=(sp_lils_row_node*)malloc(sizeof(sp_lils_row_node));         // Allocate temporary row node
            temp_c=(sp_lils_col_node*)malloc(sizeof(sp_lils_row_node));         // Allocate temporary column node
            temp_c->idx_c=col;                                  // Set index of column
            temp_c->value=value;                                // Set value of cells
            temp_c->next=NULL;                                  // Set next of column to NULL
            temp_r->idx_r=row;                                  // Set index of row
            temp_r->col_list_head=temp_c;                       // Set column head to column
            temp_r->next=cur_r->next;                           // Set next of row to next of current row
            cur_r->next=temp_r;                                 // Set next of current row to row
            mat_l->nz++;                                        // Increment non-zero cells
            return;                                             // Return
        }
    }
}

void save_lils(char * file_name,sp_lils * mat_l){
    FILE * file_ptr;                                            // Declare file pointer
    sp_lils_row_node *row_ptr;                                  // Declare variables
    sp_lils_col_node *col_ptr;                                  // Declare variables
    file_ptr=fopen(file_name,"w");                              // Set file pointer
    fprintf(file_ptr,"%d %d\n",mat_l->m,mat_l->n);              // Print total rows and total columns into file
    row_ptr=mat_l->row_list_head;                               // Set row pointer
    while (row_ptr!=NULL){                                      // Loop through rows
        col_ptr=row_ptr->col_list_head;                         // Set column pointer
        while (col_ptr!=NULL){                                  // Loop through columns
            fprintf(file_ptr,"%d %d %lf\n",row_ptr->idx_r,col_ptr->idx_c,col_ptr->value);       // Save cell info into file
            col_ptr=col_ptr->next;                              // Move to next column
        }
        row_ptr=row_ptr->next;                                  // Move to next row
    }
    fclose(file_ptr);                                           // Close file
    return;                                                     // Return
}

sp_lils * add_lils(sp_lils * matA, sp_lils * matB){
    int total_rows=matA->m, total_cols=matA->n;                                 // Declare varaibles
    double val;                                                                 // Declare variables
    sp_lils_row_node *row_ptr;                                                  // Declare variables
    sp_lils_col_node *col_ptr;                                                  // Declare variables
    sp_lils *matC=malloc(sizeof(sp_lils));                                      // Allocate matrix
    matC->m=total_rows;                                                         // Set total rows
    matC->n=total_cols;                                                         // Set total columns
    matC->nz=0;                                                                 // Set non-zero cells to 0
    matC->row_list_head=NULL;                                                   // Set row head to NULL
    row_ptr=matA->row_list_head;                                                // Set row pointer to row head
    while (row_ptr!=NULL){                                                      // Loop through rows
        col_ptr=row_ptr->col_list_head;                                         // Set column pointer
        while (col_ptr!=NULL){                                                  // Loop through columns
            set_lils(matC,row_ptr->idx_r,col_ptr->idx_c,col_ptr->value);        // Set cell in matrix
            col_ptr=col_ptr->next;                                              // Move to next column
        }
        row_ptr=row_ptr->next;                                                  // Move to next row
    }
    row_ptr=matB->row_list_head;                                                // Set row pointer
    while (row_ptr!=NULL){                                                      // Loop through rows
        col_ptr=row_ptr->col_list_head;                                         // Set column pointer
        while (col_ptr!=NULL){                                                  // Loop through columns
            if (gv_lils(matC,row_ptr->idx_r,col_ptr->idx_c)!=0){                // Check if cell exists
                val=gv_lils(matC,row_ptr->idx_r,col_ptr->idx_c);                // Set value
                set_lils(matC,row_ptr->idx_r,col_ptr->idx_c,val+col_ptr->value);// Set cell with new value
            }
            else {
                set_lils(matC,row_ptr->idx_r,col_ptr->idx_c,col_ptr->value);    // Set cell with value
            }
            col_ptr=col_ptr->next;                                              // Move to next column
        }
        row_ptr=row_ptr->next;                                                  // Move to next row
    }
    return matC;                                                                // Return matrix
}

void destroy_lils(sp_lils * mat_l){
    sp_lils_row_node *row_ptr, *temp_r;                         // Declare variables
    sp_lils_col_node *col_ptr, *temp_c;                         // Declare variables
    row_ptr=mat_l->row_list_head;                               // Set row pointer
    while (row_ptr!=NULL){                                      // Loop through rows
        col_ptr=row_ptr->col_list_head;                         // Set column pointer
        while (col_ptr!=NULL){                                  // Loop through columns
            temp_c=col_ptr;                                     // Save current cell
            col_ptr=col_ptr->next;                              // Move to next column
            free(temp_c);                                       // Free current cell
        }
        temp_r=row_ptr;                                         // Save current row
        row_ptr=row_ptr->next;                                  // Move to next row
        free(temp_r);                                           // Free current row
    }
    free(mat_l);                                                // Free matrix
    return;                                                     // Return
}

void del_node(sp_lils *mat, int row, int col){
    sp_lils_row_node *row_ptr, *cur_r;                                          // Declare varaibles
    sp_lils_col_node *col_ptr, *cur_c, *temp_col_ptr;                           // Declare varaibles
    if (gv_lils(mat,row,col)==0){                                               // Check if cell exists
        return;                                                                 // Return
    }
    else {
        row_ptr=mat->row_list_head;                                             // Set row pointer
        while (row_ptr!=NULL){                                                  // Loop through rows
            if (row_ptr->idx_r==row){                                           // Check if row is given row
                col_ptr=row_ptr->col_list_head;                                 // Set column pointer
                while (col_ptr!=NULL){                                          // Loop through columns
                    if (col_ptr->idx_c==col){                                   // Check if column is given column
                        if (col_ptr==row_ptr->col_list_head){                   // If column pointer is column head
                            row_ptr->col_list_head=col_ptr->next;               // Set column pointer to next of column
                            free(col_ptr);                                      // Free column
                            mat->nz--;                                          // Decrement non-zero cells
                            goto out;                                           // Goto out
                        }
                        else {
                            temp_col_ptr=row_ptr->col_list_head;                // Set temp column pointer
                            while (temp_col_ptr!=NULL && temp_col_ptr->idx_c!=col){             // Loop through columns
                                cur_c=temp_col_ptr;                             // Set current column less than column
                                temp_col_ptr=temp_col_ptr->next;                // Move to next column
                            }
                            cur_c->next=col_ptr->next;                          // Set next of current column to next of column
                            free(col_ptr);                                      // Free column
                            mat->nz--;                                          // Decrement non-zero cells
                            goto out;                                           // Goto out
                        }
                    }
                    col_ptr=col_ptr->next;                                      // Move to next column
                }
            }
            row_ptr=row_ptr->next;                                              // Move to next row
        }
    }
    out:
    row_ptr=mat->row_list_head;                                                 // Set row pointer
    if (row_ptr!=NULL && row_ptr->col_list_head==NULL){                         // Check if first row is empty
        mat->row_list_head=row_ptr->next;                                       // Set row head to next of row
        free(row_ptr);                                                          // Free row
    }
    while (row_ptr!=NULL && row_ptr->col_list_head!=NULL){                      // Loop through rows
        cur_r=row_ptr;                                                          // Set current row less than given row
        row_ptr=row_ptr->next;                                                  // Move to next row
    }
    if (row_ptr==NULL){                                                         // If row pointer is NULL
        return;                                                                 // Return
    }
    cur_r->next=row_ptr->next;                                                  // Set next of current row to next of row
    free(row_ptr);                                                              // Free row pointer
    return;                                                                     // Return
}
