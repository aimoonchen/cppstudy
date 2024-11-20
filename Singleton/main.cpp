// http://www.purecpp.cn/detail?id=2421
#include <iostream>
#include <dlfcn.h>

#include "impl.h"
#include "lib.h"
#include "foo.h"

int main(int _argc, char* _argv[])
{
    std::cout << __PRETTY_FUNCTION__ << ": main()" << std::endl;

    void* lib = dlopen("/app/build/libdemo.so", RTLD_LAZY);
    if(lib == nullptr)
        return std::cout << dlerror() << std::endl, 1;
    decltype(&reference) ref = (decltype(&reference))dlsym(lib, "reference");
    if(ref == nullptr)
        return std::cout << dlerror() << std::endl, 1;
    
    std::cout << __PRETTY_FUNCTION__ << ": ", singleton::plug<foo>().method();

    inject(ref);
    std::cout << __PRETTY_FUNCTION__ << ": inject()" << std::endl;

    decltype(&libfunc) func = (decltype(&libfunc))dlsym(lib, "libfunc");
    if(func == nullptr)
        return std::cout << dlerror() << std::endl, 1;
    func();
    dlclose(lib);

    std::cout << __PRETTY_FUNCTION__ << ": ~main()" << std::endl;

    return 0;
}
