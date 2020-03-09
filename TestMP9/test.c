#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width;
    int height;
    int startColumn;
    int startRow;
    int endColumn;
    int endRow;
    char ** cells;
} maze_t;

maze_t * createMaze(char * fileName);

int main()
{
        createMaze("./tests/maze1.txt");
        return 1;
}

maze_t * createMaze(char * fileName)
{
        // printf("%s",fileName);
        int width, height, col, row;
        char cell_char;
        FILE *fp;
        maze_t * mygame = malloc(sizeof(maze_t));
        fp = fopen(fileName,"r");
        fscanf(fp,"%d",&width);
        fscanf(fp,"%d",&height);
        // printf("Width=%d, Height=%d\n",width,height);
        mygame->cells = malloc(height*sizeof(char*));
        for (row=0;row<height;row++){
                (mygame->cells)[row] = malloc(width*sizeof(char));
        }
        mygame->width=width;
        mygame->height=height;
        fseek(fp,1,SEEK_CUR);
        // mygame->cells[0][0]='S';
        // printf("Width=%d, Height=%d,",mygame->width,mygame->height);
        // printf("%c",mygame->cells[0][0]);
        // cell_char=fgetc(fp);
        // mygame->cells[4][0]=cell_char;
        // printf("%c",mygame->cells[4][0]);
        // for (row=0;row<height;row++){
        //         for (col=0;col<width;col++){
        //                 printf("Row-%d,Col-%d,Cell-%c",row,col,mygame->cells[row][col]);
        //         }
        //         printf("\n");
        // }
        for (row=0;row<height;row++){
                for (col=0;col<width;col++){
                        cell_char=fgetc(fp);
                        if (cell_char=='\n'){
                                cell_char=fgetc(fp);
                        }
                        printf("Row-%d,Col-%d,Cell-%c",row,col,cell_char);
                        mygame->cells[row][col]=cell_char;
                }
                printf("\n");
        }
        // fclose(fp);
        // for (col=0;col<width;col++){
        //         for (row=0;row<height;row++){
        //                 cell_char=*(mygame->cells[row*width+col]);
        //                 if (cell_char=='S'){
        //                         mygame->startRow=row;
        //                         mygame->startColumn=col;
        //                 }
        //                 if (cell_char=='E'){
        //                         mygame->endRow=row;
        //                         mygame->endColumn=col;
        //                 }
        //         }
        // }
        return mygame;
        // return NULL;
}
