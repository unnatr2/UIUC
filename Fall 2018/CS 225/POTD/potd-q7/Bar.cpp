#include "Bar.h"
#include <string>

using namespace potd;
using namespace std;

void Bar::_copy(const Bar & other){
    this->f_=new Foo(*other.f_);
}

void Bar::_destroy(){
    delete this->f_;
}

Bar::Bar(string s){
    this->f_=new Foo(s);

}
Bar::Bar(const Bar & other){
    _copy(other);
}
Bar & Bar::operator=(const Bar & other){
    _destroy();
    _copy(other);
    return *this;
}
Bar::~Bar(){
    _destroy();
}
string Bar::get_name(){
    return this->f_->get_name();
}