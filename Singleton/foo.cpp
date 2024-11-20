#include <iostream>

#include "impl.h"
#include "foo.h"
#include "bar.h"

foo::foo(){std::cout << __PRETTY_FUNCTION__ << ": ", singleton::plug<bar>().hello();}
foo::~foo(){std::cout << __PRETTY_FUNCTION__ << ": ", singleton::plug<bar>().hello();}
void foo::method(){std::cout << __PRETTY_FUNCTION__ << "{" << value++ << "}" << std::endl;}
