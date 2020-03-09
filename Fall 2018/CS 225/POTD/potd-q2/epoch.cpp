#include "epoch.h"


int hours(time_t sec_since_epoch){
    return sec_since_epoch/3600;
}
int days(time_t sec_since_epoch){
    return sec_since_epoch/86400;
}
int years(time_t sec_since_epoch){
    return sec_since_epoch/31536000;
}
