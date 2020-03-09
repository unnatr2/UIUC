#include "Food.h"

Food::Food(){
    name_="apples";
    quantity_=5;
}
std::string Food::get_name(){
    return name_;
}
double Food::get_quantity(){
    return quantity_;
}
void Food::set_name(std::string n){
    name_=n;
}
void Food::set_quantity(double q){
    quantity_=q;
}
