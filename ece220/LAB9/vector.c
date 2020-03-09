#include "vector.h"
#include <stdlib.h>


vector_t * createVector(int initialSize)
{
        vector_t*vector=malloc(sizeof(vector_t));
        vector->maxSize=initialSize;
        vector->array=malloc(initialSize*sizeof(int));
        return vector;
}

void destroyVector(vector_t * vector)
{
        free (vector->array);
        free(vector);
}

void resize(vector_t * vector)
{
        vector_t*vector1=malloc(sizeof(vector_t));
        vector1->array=realloc(vector->array,sizeof(vector->array)*2);
        vector->array=vector1->array;
        vector->maxSize*=2;
}

void push_back(vector_t * vector, int element)
{
        int size=vector->size;                          // i
        if (size == vector->maxSize){
                vector_t*vector1=malloc(sizeof(vector_t));
                vector1->array=realloc(vector->array,sizeof(vector->array)+1);
                vector->array=vector1->array;
                vector->maxSize+=1;
                vector->array[size]=element;
                // for (i=0;i<vector->maxSize;i++){
                //         printf("%d ",vector->array[i]);
                // }
                // printf("\n");
                vector->size+=1;
        }
        else {
                vector->array[size]=element;
                // for (i=0;i<vector->maxSize;i++){
                //         printf("%d ",vector->array[i]);
                // }
                // printf("\n");
                vector->size+=1;
        }

}

int pop_back(vector_t * vector)
{
        int result, size=vector->size;
        if (size==0){
                return 0;
        }
        else {
                result=vector->array[size-1];
                vector->array[size-1]=0;
                vector->size-=1;
                return result;
        }
}

int access(vector_t * vector, int index)
{
        int result, size=vector->size;
        if (index<0 || index>vector->maxSize){
                return 0;
        }
        else {
                result=vector->array[index];
                return result;
        }
}
