#include "Food.h"

void increase_quantity(Food* input){
    double input_quantity=input->get_quantity();
    input_quantity+=1;
    input->set_quantity(input_quantity);
}