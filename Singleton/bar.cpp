#include <iostream>

#include "bar.h"

bar::bar(){std::cout << __PRETTY_FUNCTION__ << std::endl;}
bar::~bar(){std::cout << __PRETTY_FUNCTION__ << std::endl;}
void bar::hello(){std::cout << __PRETTY_FUNCTION__ << "{" << value << "}" << std::endl;}
