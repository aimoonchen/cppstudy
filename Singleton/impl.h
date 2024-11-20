#include "singleton.h"

struct foo;
struct bar;

using singleton = patterns::singleton<bar, foo>;

void inject(void (*_dynamic)(std::size_t, void*));
