#include "potd.h"
#include <iostream>
#include <cmath>
using namespace std;

namespace potd {
    int *raise(int *arr){
        int* new_arr = new int;
        int i=0;
        while(arr[i]>0){
            if (arr[i+1]<0){
                new_arr[i]=arr[i];
            }
            else{
            new_arr[i]=pow(arr[i],arr[i+1]);
            }
            i++;
        }
        new_arr[i]=arr[i];
        return new_arr;
    }
}