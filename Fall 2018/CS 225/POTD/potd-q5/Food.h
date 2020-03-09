#ifndef _FOOD_H
#define _FOOD_H
#include <string>

class Food {
    public:
        Food();
        std::string get_name();
        double get_quantity();
        void set_name(std::string n);
        void set_quantity(double q);
    private:
    std::string name_;
    double quantity_;
};



#endif