#include "Thing.h"
#include <iostream>

namespace potd {
    Thing::Thing(int size){
        props_ct_=0;
        props_max_=size;
        properties_= new std::string[size];
        values_= new std::string[size];
    }

    Thing::Thing(const Thing &other){
        _copy(other);
    }

    Thing & Thing::operator=(const Thing &other){
        _destroy();
        _copy(other);
        return *this;
    }

    Thing::~Thing(){
        _destroy();
    }

    int Thing::set_property(std::string name, std::string value){
        for (int i=0; i<props_max_; i++){
            if (properties_[i]==name){
                values_[i]=value;
                return i;
            }
        }
        if (props_ct_==props_max_){
            return -1;
        }
        properties_[props_ct_]=name;
        values_[props_ct_]=value;
        props_ct_++;
        return props_ct_-1;
    }

    std::string Thing::get_property(std::string name){
        for (int i=0; i<props_max_; i++){
            if (properties_[i]==name){
                return values_[i];
            }
        }
        return "";
    }

    void Thing::_copy(const Thing &other){
        props_ct_=other.props_ct_;
        props_max_=other.props_max_;
        properties_= new std::string[props_max_];
        values_= new std::string[props_max_];
        for (int i=0; i<props_max_; i++){
            properties_[i]=other.properties_[i];
            values_[i]=other.values_[i];
        }
    }

    void Thing::_destroy(){
        delete [] properties_;
        delete [] values_;
        props_ct_=0;
        props_max_=0;
        properties_=NULL;
        values_=NULL;
    }
}