#include<iostream>
#include "C_numbers.h"


std::ostream& operator<<(std::ostream &o, const C_number &z){
    if(z.im == 0)
        o<<z.re;
    else
        o<<z.re<<(z.im>0? "+" : "-" )<<std::abs(z.im)<<"*i";
    return o;
}
