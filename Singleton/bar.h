#ifndef BAR_H
#define BAR_H

struct bar final{
    bar();
    ~bar();
    void hello();
private:
    int value = 514;
};

#endif
