#include <iostream>

#include "lib.h"
#include "impl.h"
#include "foo.h"

void reference(std::size_t _size, void* _array)
{
    singleton::reference(_size, _array);
}

void libfunc()
{
    std::cout << __PRETTY_FUNCTION__ << ": ", singleton::plug<foo>().method();
}
