#include "sliding.h"
/*  Slide all values of array up
*/
void slide_up(int* my_array, int rows, int cols){
        int n,m,t;
        for (n=0;n<cols;n++){
                for (m=0;m<rows;m++){
                        if (my_array[m*cols+n]!=-1){
                                for (t=0;t<m;t++){
                                        if (my_array[t*cols+n]==-1){
                                                my_array[t*cols+n]=my_array[m*cols+n];
                                                my_array[m*cols+n]=-1;
                                        }
                                }
                        }
                }
        }
        return;
}
