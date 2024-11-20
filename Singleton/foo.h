#ifndef FOO_H
#define FOO_H

struct foo final{
    foo();
    ~foo();
    void method();
private:
    int value = 114;
};

#endif
